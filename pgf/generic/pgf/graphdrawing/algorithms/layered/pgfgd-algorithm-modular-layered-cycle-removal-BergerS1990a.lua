-- Copyright 2011 by Jannis Pohlmann
--
-- This file may be distributed and/or modified
--
-- 1. under the LaTeX Project Public License and/or
-- 2. under the GNU Public License
--
-- See the file doc/generic/pgf/licenses/LICENSE for more information

-- @release $Header: /cvsroot/pgf/pgf/generic/pgf/graphdrawing/algorithms/layered/pgfgd-algorithm-modular-layered-cycle-removal-BergerS1990a.lua,v 1.1 2011/07/15 15:53:28 jannis-pohlmann Exp $

pgf.module("pgf.graphdrawing")



CycleRemovalBergerS1990a = {}
CycleRemovalBergerS1990a.__index = CycleRemovalBergerS1990a



function CycleRemovalBergerS1990a:new(graph)
  local algorithm = {
    graph = graph,
  }
  setmetatable(algorithm, CycleRemovalBergerS1990a)
  return algorithm
end



function CycleRemovalBergerS1990a:run()
  -- remember edges that were removed
  local removed = {}

  -- remember edges that need to be reversed
  local reverse = {}

  -- iterate over all nodes of the graph
  for node in table.value_iter(self.graph.nodes) do
    -- get all outgoing edges that have not been removed yet
    local out_edges = table.filter_values(node:getOutgoingEdges(), function (edge)
      return not removed[edge]
    end)

    -- get all incoming edges that have not been removed yet
    local in_edges = table.filter_values(node:getIncomingEdges(), function (edge)
      return not removed[edge]
    end)

    if #out_edges >= #in_edges then
      -- we have more outgoing than incoming edges, reverse all incoming 
      -- edges and mark all incident edges as removed
      
      for edge in table.value_iter(out_edges) do
        removed[edge] = true
      end
      for edge in table.value_iter(in_edges) do
        reverse[edge] = true
        removed[edge] = true
      end
    else
      -- we have more incoming than outgoing edges, reverse all outgoing
      -- edges and mark all incident edges as removed

      for edge in table.value_iter(out_edges) do
        reverse[edge] = true
        removed[edge] = true
      end
      for edge in table.value_iter(in_edges) do
        removed[edge] = true
      end
    end
  end

  -- mark edges as reversed
  for edge in table.key_iter(reverse) do
    edge.reversed = true
  end
end
