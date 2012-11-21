-- Copyright 2010 by Renée Ahrens, Olof Frahm, Jens Kluttig, Matthias Schulz, Stephan Schuster
-- Copyright 2012 by Till Tantau
--
-- This file may be distributed an/or modified
--
-- 1. under the LaTeX Project Public License and/or
-- 2. under the GNU Public License
--
-- See the file doc/generic/pgf/licenses/LICENSE for more information

-- @release $Header: /cvsroot/pgf/pgf/generic/pgf/graphdrawing/lua/pgf/gd/examples/pgf.gd.examples.SimpleEdgeDemo.lua,v 1.2 2012/11/20 23:31:58 tantau Exp $


---
-- @section subsection {How To Generate Edges Inside an Algorithm}

local _


-- Imports
local InterfaceToAlgorithms = require "pgf.gd.interface.InterfaceToAlgorithms"
local declare               = InterfaceToAlgorithms.declare

-- The class object
local SimpleEdgeDemo = {}


---
declare {
  key = "simple edge demo layout",
  algorithm = SimpleEdgeDemo,

  summary = "This algorithm shows how edges can be created by an algorithm.",
  documentation = [["  
       For its job, the algorithm uses the function |createEdge|, which can be
       called during the run of the algorithm to create edges in the
       syntactic graph. The algorithm first does exactly the same as the
       simple demo layout, then it creates an edge for every node where the
       |new edge to| option is set. You will see in the code how this
       option is declared and how we use it to look up a vertex in the
       graph by its name. 
      
\begin{codeexample}[]
\tikz [simple edge demo layout]
\graph [radius=2cm] {
  a, b, c, d, e, f;
        
  e -> [red] f; % normal edge
  
  % Edges generated by the algorithm:
  a[new edge to=b]; 
  b[new edge to=d];
  c[new edge to=f];
};
\end{codeexample}
      
       And the algorithm:
\begin{codeexample}[code only]
-- File pgf.gd.examples.SimpleEdgeDemo
       
-- Imports
local InterfaceToAlgorithms = require "pgf.gd.interface.InterfaceToAlgorithms"
local declare               = InterfaceToAlgorithms.declare

-- The class object
local SimpleEdgeDemo = {}

---
declare {
  key = "simple edge demo layout",
  algorithm = SimpleEdgeDemo,
  summary = "This algorithm shows...",
}
\end{codeexample}
       
       Next comes the declaration of the new option |new edge to|:
      
\begin{codeexample}[code only]
---
declare {
  key = "new edge to",
  type = "string",
  summary = "This option takes the name of a vertex..."
}
\end{codeexample}
      
       Finally, the algorithm's code:
      
\begin{codeexample}[code only]
function SimpleEdgeDemo:run()
  -- As in a SimpleDemo:
  ...
  -- Now add some edges:
  for _,tail in ipairs(g.vertices) do
    local name = tail.options['new edge to']
    if name then
      local node = InterfaceToAlgorithms.findVertexByName(name)
      if node and self.digraph:contains(node) then
	InterfaceToAlgorithms.createEdge (self, tail, node)
      end
    end
  end
end
\end{codeexample}
  "]]
}

---
declare {
  key = "new edge to",
  type = "string",

  summary = [["  
       This option takes the name of a vertex. An edge leading to this
       vertex is added to the syntactic digraph.
   "]]
 }


function SimpleEdgeDemo:run()
  
  -- As in a SimpleDemo:
  local g = self.digraph
  local alpha = (2 * math.pi) / #g.vertices

  for i,vertex in ipairs(g.vertices) do
    local radius = vertex.options['radius'] or g.options['radius']
    vertex.pos.x = radius * math.cos(i * alpha)
    vertex.pos.y = radius * math.sin(i * alpha)
  end

  -- Now add some edges:
  for _,tail in ipairs(g.vertices) do
    local name = tail.options['new edge to']
    if name then
      local node = InterfaceToAlgorithms.findVertexByName(name)
      if node and self.digraph:contains(node) then
	InterfaceToAlgorithms.createEdge (self, tail, node)
      end
    end
  end
end

return SimpleEdgeDemo