-- Copyright 2011 by Jannis Pohlmann
-- Copyright 2012 by Till Tantau
--
-- This file may be distributed an/or modified
--
-- 1. under the LaTeX Project Public License and/or
-- 2. under the GNU Public License
--
-- See the file doc/generic/pgf/licenses/LICENSE for more information

-- @release $Header: /cvsroot/pgf/pgf/generic/pgf/graphdrawing/lua/pgf/gd/model/pgf.gd.model.Cluster.lua,v 1.1 2012/04/19 15:22:29 tantau Exp $



--- The Cluser class defines a model of a cluster inside a graph.
--
--

local Cluster = {}
Cluster.__index = Cluster


-- Namespace

local model   = require "pgf.gd.model"
model.Cluster = Cluster



--- TODO Jannis: Add documentation for this class.
--
function Cluster.new(name)
  local cluster = {
    name = name,
    nodes = {},
    contains_node = {},
  }
  setmetatable(cluster, Cluster)
  return cluster
end



function Cluster:getName()
  return self.name
end



function Cluster:addNode(node)
  if not self:findNode(node) then
    self.contains_node[node] = true
    self.nodes[#self.nodes + 1] = node
  end
end



function Cluster:findNode(node)
  return self.contains_node[node]
end




-- Done

return Cluster