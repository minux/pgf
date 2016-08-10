-- Copyright 2015 by Malte Skambath
--
-- This file may be distributed an/or modified
--
-- 1. under the LaTeX Project Public License and/or
-- 2. under the GNU Public License
--
-- See the file doc/generic/pgf/licenses/LICENSE for more information
-- 

-- Imports
require "pgf.gd.trees.ChildSpec"

local Digraph                 = require "pgf.gd.model.Digraph"
local Vertex                  = require "pgf.gd.model.Vertex"

local declare                 = require("pgf.gd.interface.InterfaceToAlgorithms").declare
local layered                 = require "pgf.gd.layered"
local tlayered                = require "pgf.gd.experimental.evolving.layered"
local SpanningTreeComputation = require "pgf.gd.trees.SpanningTreeComputation" 
local lib                     = require "pgf.gd.lib"

local Storage                 = require "pgf.gd.lib.Storage"
local PriorityQueue           = require "pgf.gd.lib.PriorityQueue"

local Supergraph              = require "pgf.gd.experimental.evolving.Supergraph"
--local Borders                 = require "pgf.gd.experimental.evolving.Borders"

local LayoutPipeline          = require "pgf.gd.control.LayoutPipeline"
local Direct                  = require "pgf.gd.lib.Direct"

--
--
local Skambath2016 = {}


---
declare {
   key            = "animated tree layout",
   algorithm      = Skambath2016,
   postconditions = {
     upward_oriented = true
   },
   documentation_in = "pgf.gd.evolving.doc"
}

---
declare {
  key = "animated binary tree layout",
  use = {
    { key = "animated tree layout" },
    { key = "minimum number of children", value = 2 },
    { key = "significant sep", value = 0 },
  },

  documentation_in = "pgf.gd.evolving.doc"
}

---
declare {
  key = "extended animated binary tree layout",
  use = {
    { key = "animated tree layout" },
    { key = "minimum number of children", value=2 },
    { key = "missing nodes get space" },
    { key = "significant sep", value = 0 }
  },
  documentation_in = "pgf.gd.evolving.doc"
}





-- Helpfunctions

---
-- Borders models the borderlines / the line of border vertices
-- of a tree or subtree structure which can change over time.
-- Each ordered and rooted tree or subtree has vertices per layer for which they
-- are on the outer places. On the left or respectivly on the right.
-- The field |left| and |right| stores the both borderlines.
-- A \emph{borderline} is an array. This array contains for each layer of the tree
-- a table mapping a given snapshot to the related border-vertex of the layer
-- in the snapshot.
-- 
-- @field left 
-- @field right
--
local Borders = {}
Borders.__index = Borders


-- Namespace

--require("pgf.gd.experimental.evolving").Borders = Borders


--- Create a new tree border description.
--
--
function Borders.new()
  return setmetatable( {left = {}, right = {}}, Borders )
end

function Borders:addBoth(layer, snapshot, vertex)
  local lleft  = self.left[layer] or {}
  local lright = self.right[layer] or {}
  assert(not lleft[snapshot] and not lright[snapshot], "borders already defined for given layer and snapshot")
  lleft[snapshot]   = vertex
  lright[snapshot]  = vertex
  self.left[layer]  = lleft
  self.right[layer] = lright
end

function Borders:appendBelow(level, borders)
  assert(borders, "invalid argument for borders. Value must not be 'nil'.")
  assert((#self.left == #self.right) and (#self.left == level), "cannot be appended")
  local new_level = borders:getLevel()
  for i = 1, new_level do
    self.left[i + level] = borders.left[i]
    self.right[i + level] = borders.right[i]
  end
end

---
--
-- @return the number of levels in which bordervertices exists.
function Borders:getLevel()
  assert(#self.left == #self.right, "different levels")
  return #self.left
end

function Borders.copyBordersAtSnapshot(source, target, snapshot)
  local source_level = source:getLevel()
  for i = 1, source_level do
    level_border_left  = target.left[i]  or {}
    level_border_right = target.right[i] or {}
    assert(not level_border_left[snapshot] and not level_border_right[snapshot],
	   "border for a given snapshat already defined")
    level_border_left[snapshot]  = source.left[i][snapshot]
    level_border_right[snapshot] = source.right[i][snapshot]
    target.left[i]  = level_border_left
    target.right[i] = level_border_right
  end
end

--
-- Adapt existing borders with the borders of a group which will be
-- placed on the right next to the existing borders. For each level
-- and time in which the group has a border the borders  on the right
-- will be replaced with this border. If it is not existing in the old
-- borders then also the left border will be adapted 
--
-- @param borders The existing borders. These will be modified with
-- the borders of
--
-- @param group_borders The borders of the group
--
-- @param snapshots All snapshots in which checking for distances is necessary.
--
local function adapt_borders(borders, group_borders, snapshots, ignore_dummies)
  for level = 1, group_borders:getLevel() do
    local l = borders.left[level] or {}
    local r = borders.right[level] or {}
    for _, s in pairs(snapshots) do
      if ignore_dummies then
	local gls,grs = group_borders.left[level][s], group_borders.right[level][s]
	if gls~=nil then
	  if gls.kind~="dummy" then
	    l[s] = l[s] or gls 
	  end
	end
	if grs~=nil then
	  if grs.kind~="dummy" then
	    r[s] = grs or r[s]
	  end
	end
      else
	l[s] = l[s] or group_borders.left[level][s]
	r[s] = group_borders.right[level][s] or r[s]
      end
      
    end
    borders.left[level] = l
    borders.right[level] = r
  end
  
end

--
-- Shift all vertices of a group and their descendants
-- for a certain length into horizontal direction.
--
-- @param shift       the length all involved vertices
--                    should be shifted in horizontal direction
--
--
-- @param group       A group of the spanning trees that should be shifted.
--                    A group entry has to map snapshots to root nodes
--
-- @param snapshots   An array of at least all snapshots in which the group
--                    has a vertex
--
-- @param descendants A table or Storage storing the list of descendants
--                    for each vertex
--
-- @return the highest x-coordinate of vertices in the group after the shift.
--         If there is no vertex which was shifted then -math.huge
--         will be returned
--
local function shift_group(shift, group, snapshots, descendants)
  assert(group,"no group passed")
  assert(shift~=math.huge and shift ~=-math.huge,
	 "shift must be a valid finit length")
  local shifted = {} -- remember which vertex was shifted during this run
  local anchor = -math.huge
  for _, s in ipairs(snapshots) do
    local v = group[s] 
    if not shifted[v] and v then 
      v.pos.x = v.pos.x + shift
      shifted[v] = true

      -- also shift all descendants of the group vertex 
      for _, d in ipairs(descendants[v]) do
        if not shifted[d] then
          d.pos.x = d.pos.x + shift
          shifted[d] = true
        end
      end
      anchor = math.max(anchor, v.pos.x )
    end   
  end
  return anchor
end

--
-- Traverse through the spanneng tree |tree| of a snapshot and sort
-- the child nodes into groups. A group summarises for a given parent
-- node all  children node over time that are at the same child
-- position. The k-th child group groups[i] maps each snapshot to the
-- k-th node in the related snapshot.
--
--
-- @field supergraph the supergraph
-- 
-- @field tree the tree or spanning tree to decide the indices of the
-- child nodes
--
-- @field childgroups a Storage which contains the list of childgroups
-- for each supernode
--
-- @field snapshot 
--
local function precompute_childgroups(supergraph, tree, node, childgroups, snapshot)
  local outgoings  = tree:outgoing(node)
  if #outgoings > 0 then
    local supervertex = supergraph:getSupervertex(node)
    local groups     = childgroups[supervertex] or {}
    for i, e in ipairs(outgoings) do
      
      group = groups[i] or {}
      assert(e.head, "no edge")
      group[snapshot] = e.head
      groups[i] = group
      precompute_childgroups(supergraph, tree, e.head, childgroups, snapshot)
    end
    assert(supervertex, "no mv")
    childgroups[supervertex] = groups
  end
end


-- Implementation

function Skambath2016:run()
  local layers       = Storage.new()
  local descendants  = Storage.new()
  local childgroups  = Storage.new()

  local phases = self.digraph.options.algorithm_phases
  
  local so_class = phases['supergraph optimization']
  local ga_class = phases['evolving graph animation']
  local cr_class = phases['temporal cycle removal']
 
  self.extended_version = self.digraph.options['missing nodes get space']
  self.supergraph           = Supergraph.generateSupergraph(self.digraph)
  local supergraph_original = Supergraph.generateSupergraph(self.digraph)

  -- optimize graph by splitting nodes by trivial criteria
  so_class.new {
    main_algorithm = self,
    supergraph     = self.supergraph,
    digraph        = self.digraph,
    ugraph         = self.ugraph
  }:run()
  
  
  self:precomputeSpanningTrees()

  -- Resolve cyclic dependencies if exists.
  cr_class.new {
    main_algorithm = self,
    supergraph     = self.supergraph,
    digraph        = self.digraph,
  }:run()
  
  self:precomputeDescendants(layers, descendants)
  self:precomputeChildgroups(childgroups)
  self:computeHorizontalLayout(childgroups, descendants)
  
  
  -- vertical positions  
  tlayered.arrange_layers_by_baselines(layers,
				       self.adjusted_bb,
				       self.ugraph,
				       self.supergraph.snapshots,
				       self.supergraph.vertex_snapshots)

  -- animate graph
  ga_class.new {
    main_algorithm = self,
    supergraph     = supergraph_original,
    digraph        = self.digraph,
    ugraph         = self.ugraph
  }:run()
end

--
-- Compute the required shift value for a second tree to garuantee
-- a required node distance.
-- @field right_borders The Border data structure for the right border of
--                      the left tree
-- @field left_borders  The Border data structure for the left border of
--                      the right tree
-- @field selected_snapshots if you set this value with an array of snapshots
--                      only the predefined snapshots are used in the border
--                      computation.
--
function Skambath2016:computeMinShift(right_borders, left_borders, selected_snapshots)
  local shift       = -math.huge
  local max_level   = math.min(#right_borders, #left_borders)
  local first_shift = 0
  local snapshots   = selected_snapshots or self.supergraph.snapshots
  
  for layer = 1, max_level do
    local rb, lb = right_borders[layer], left_borders[layer]
    for _,s in ipairs(snapshots)  do
      
      local v1,v2 = rb[s],lb[s]
      if v1 and v2 then
        local local_shift = layered.ideal_sibling_distance(self.adjusted_bb, self.ugraph, v1, v2) + v1.pos.x - v2.pos.x
        shift = math.max(shift, local_shift)
      end
    end
    if layer == 1 then
      first_shift = shift
    end
  end

  local is_significant = false

  if max_level > 1 and  shift<=first_shift then
    -- if the necessary shift of the subtrees
    -- is the minimum required shift between two
    -- nodes than a node is significant
    is_significant = true
  end

  if shift <= -math.huge then
    shift = 0
  end

  if is_significant then
    shift = shift + self.ugraph.options['significant sep']
  end
  
  return shift
end


--
-- The main algorithm: This method computes the layout for each vertex.
-- For this all supervertices are visited in a topological order to their dependency.
-- If a . This requires the supergraph to be acyclic. If this is not the case
-- the calling process has to remove all cycles otherwise the x-coordinate will
-- not be computed for every vertex.
--
function Skambath2016:computeHorizontalLayout(groups, descendants)
  local subtree_borders = Storage.new()
  local dep_counter = {}
  local visited   = {}
  local queue     = PriorityQueue.new()
  local dependency_graph    = Digraph.new()
  for _, vertex in ipairs(self.supergraph.vertices) do
    dep_counter[vertex] = 0
    dependency_graph:add {vertex}
  end

  -- 1. Initialize Dependencies
  
  for _, snapshot in ipairs(self.supergraph.snapshots) do
    for _, spanning_tree in ipairs(snapshot.spanning_trees) do
      for _, arc in ipairs(spanning_tree.arcs) do
	
	local head = self.supergraph:getSupervertex(arc.head)
	local tail = self.supergraph:getSupervertex(arc.tail)

	if(head and tail) then
	  if not dependency_graph:arc(tail, head) then
	    dependency_graph:connect(tail, head)
	    dep_counter[tail] = dep_counter[tail] + 1
	  end
	end
      end
    end
  end

  -- 2. Find independent vertices
  for _, vertex in ipairs(dependency_graph.vertices) do
    local outgoings = dependency_graph:outgoing(vertex)
    if #outgoings == 0 then
      queue:enqueue(vertex, 1)
    end
  end
  
  -- 2. 
  while not queue:isEmpty() do
    local vertex = queue:dequeue()  
    local vertex_snapshots = self.supergraph:getSnapshots(vertex)
    
    -- a. Resolve dependencies on this vertex:
    local incomings = dependency_graph:incoming(vertex)
    for _, e in ipairs(incomings) do     
      dep_counter[e.tail] = dep_counter[e.tail] - 1
      if dep_counter[e.tail] == 0 then
	queue:enqueue(e.tail, 1)
      end   
    end

    -- b. Compute borders of this supervertex:
    local vertex_borders = Borders.new()
    for _, s in ipairs(vertex_snapshots) do
      local snapshot_vertex = self.supergraph:getSnapshotVertex(vertex, s)
      vertex_borders:addBoth(1, s, snapshot_vertex)
      snapshot_vertex.pos.x = 0
      snapshot_vertex.pos.y = 0
    end
    
    local vertex_groups = groups[vertex]
    local last_pos_x = 0
    if vertex_groups then
      -- c. Compute borders of groups:
      local all_group_borders = {}
      for i, group in ipairs(vertex_groups) do
	local group_boders = Borders.new()
	for _,s in ipairs(vertex_snapshots) do
	  local child = group[s]
	  if child then
	    local child_borders
	    if not (child.kind == "dummy") then
	      local superchild = self.supergraph:getSupervertex(child)
	      child_borders = subtree_borders[superchild] or Borders.new()
	    else
	      child_borders = Borders.new()
	      child_borders:addBoth(1, s, child)
	    end
	    assert(child.pos~=math.huge, "invalid child pos")
	    shift_group(-child.pos.x,{[s]=child},{[1]=s}, descendants)
	    Borders.copyBordersAtSnapshot(child_borders, group_boders, s)
	  end
	end
	all_group_borders[i] = group_boders
      end
      
      -- d. Place groups and merge borders of groups:
      local last_group = nil
      last_pos_x = 0
      local merged_borders = Borders.new()
      local final_borders  = Borders.new()
      for i, group in ipairs(vertex_groups) do
	local group_borders = all_group_borders[i]
	if  last_group_borders then
	  -- i.   compute minimal shift

	  local shift
	  shift = self:computeMinShift(merged_borders.right, group_borders.left)

	  
	  assert(shift ~= math.huge and shift~=-math.huge, "invalid shift")

	  -- ii.  shift group
	  local anchor = shift_group(shift, group,vertex_snapshots, descendants)
	  last_pos_x = anchor
	end
	last_group_borders = group_borders
	
	-- iii. adapt borders
	adapt_borders(merged_borders,
                      group_borders,
                      self.supergraph.snapshots)
	adapt_borders(final_borders,
		      group_borders,
                      self.supergraph.snapshots,
                      not self.extended_version)
      end -- for (group)
      vertex_borders:appendBelow(1, final_borders)
    end
    -- e. store borders:
    assert(last_pos_x~=math.huge and last_pos_x~=-math.huge, "invalid position")
    local x = ((last_pos_x) - 0)/2 + 0
    assert(x~=math.huge and x~=-math.huge, "invalid position")
    for _,s in ipairs(vertex_snapshots) do
      local snapshot_vertex = self.supergraph:getSnapshotVertex(vertex, s)
      snapshot_vertex.pos.x = x
    end
    
    subtree_borders[vertex] = vertex_borders 
  end

  -- align roots 
  for _, s in ipairs(self.supergraph.snapshots) do
    local lastroot
    local rborder 
    for i, spanning_tree in ipairs(s.spanning_trees) do
      local root = spanning_tree.root
      local rootborders = subtree_borders[self.supergraph:getSupervertex(root)]
      shift_group(-root.pos.x,{[s]=root},{[1]=s}, descendants)
      if i>1 then
	local l = subtree_borders[self.supergraph:getSupervertex(lastroot)]
	local r = rootborders
	shift = math.max(self:computeMinShift(l.right, r.left, {[1]=s}),
			 self:computeMinShift(rborder.right,r.left, {[1]=s}))
	shift_group(shift,{[s]=root},{[1]=s}, descendants)
      else
        rborder = Borders.new()
      end
      adapt_borders(rborder,rootborders,self.supergraph.snapshots,false)
      lastroot = root
    end
  end
end


function Skambath2016:precomputeTreeDescendants(tree, node, depth, layers, descendants)
  local my_descendants = { node }

  for _,arc in ipairs(tree:outgoing(node)) do
    local head = arc.head
    self:precomputeTreeDescendants(tree, head, depth+1, layers, descendants)
    for _,d in ipairs(descendants[head]) do
      my_descendants[#my_descendants + 1] = d
    end
  end
  layers[node] = depth
  descendants[node] = my_descendants
end

function Skambath2016:precomputeDescendants(layers, descendants)
  for _,snapshot in ipairs(self.supergraph.snapshots) do
    for _, spanning_tree in ipairs(snapshot.spanning_trees) do
      self:precomputeTreeDescendants(spanning_tree, spanning_tree.root, 1, layers, descendants)
    end
  end
end


--
-- 
--
function Skambath2016:precomputeChildgroups(childgroups)
  for _,s in ipairs(self.supergraph.snapshots) do
    for _,spanning_tree in ipairs(s.spanning_trees) do
      precompute_childgroups(self.supergraph, spanning_tree, spanning_tree.root, childgroups, s)
    end
  end
end

--
-- Compute a spanning tree for each snapshotgraph and appends
-- the result for a snapshot s to s.spanning_tree.
--
function Skambath2016:precomputeSpanningTrees()
  local events = assert(self.scope.events,
			"no events found for the spanning tree computation")
  
  for i, s in ipairs(self.supergraph.snapshots) do
    -- The involved snapshot graph:
    local s_copy = Digraph.new(s)
    for _,a in ipairs(s.arcs) do
      local new_a = s_copy:connect(a.tail,a.head)
      new_a.syntactic_edges = a.syntactic_edges
    end
    s.spanning_trees = s.spanning_trees or {}
    -- Step 1: Decompose the snapshot into its connected components
    local syntactic_components = LayoutPipeline.decompose(s_copy)
    for i, syntactic_component in ipairs (syntactic_components) do
      local tree = SpanningTreeComputation.computeSpanningTree(syntactic_component, true, events)
      s.spanning_trees[i] = tree
--      texio.write("\n"..i .. " |")
--      pgf.debug{syntactic_component}
--      if i == 1 then
--	s.spanning_tree = tree
--      end
    end
  end
end

return Skambath2016




