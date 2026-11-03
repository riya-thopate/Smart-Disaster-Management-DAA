import { locations, roads, previewRoute, type Road } from '@/data/mock';

const adjacency = new Map<string, string[]>();
for (const location of locations) adjacency.set(location.id, []);
for (const road of roads) {
  adjacency.get(road.from)?.push(road.to);
  adjacency.get(road.to)?.push(road.from);
}

export type TraversalResult = {
  algorithm: 'BFS' | 'DFS';
  source: string;
  order: string[];
  levels: number[];
};

export function runBFS(source: string): TraversalResult {
  const visited = new Set<string>();
  const queue: Array<{ id: string; level: number }> = [{ id: source, level: 0 }];
  const order: string[] = [];
  const levels: number[] = [];
  visited.add(source);
  while (queue.length) {
    const current = queue.shift()!;
    order.push(current.id);
    levels.push(current.level);
    for (const next of adjacency.get(current.id) ?? []) {
      if (!visited.has(next)) {
        visited.add(next);
        queue.push({ id: next, level: current.level + 1 });
      }
    }
  }
  return { algorithm: 'BFS', source, order, levels };
}

export function runDFS(source: string): TraversalResult {
  const visited = new Set<string>();
  const order: string[] = [];
  const levels: number[] = [];
  function visit(id: string, level: number) {
    visited.add(id);
    order.push(id);
    levels.push(level);
    for (const next of adjacency.get(id) ?? []) {
      if (!visited.has(next)) visit(next, level + 1);
    }
  }
  visit(source, 0);
  return { algorithm: 'DFS', source, order, levels };
}

export function runRoute(start: string, end: string, criteria: 'distance' | 'time' | 'capacity') {
  return previewRoute(start, end, criteria);
}

export function runMST() {
  const parent = new Map<string, string>();
  const rank = new Map<string, number>();
  const find = (x: string): string => {
    if (!parent.has(x)) parent.set(x, x);
    const p = parent.get(x)!;
    if (p === x) return x;
    const root = find(p);
    parent.set(x, root);
    return root;
  };
  const unite = (a: string, b: string) => {
    let ra = find(a), rb = find(b);
    if (ra === rb) return false;
    const rA = rank.get(ra) ?? 0;
    const rB = rank.get(rb) ?? 0;
    if (rA < rB) [ra, rb] = [rb, ra];
    parent.set(rb, ra);
    if (rA === rB) rank.set(ra, rA + 1);
    return true;
  };
  const sorted = [...roads].sort((a, b) => a.distance - b.distance);
  const selected: Road[] = [];
  let totalDistance = 0;
  for (const road of sorted) {
    if (unite(road.from, road.to)) {
      selected.push(road);
      totalDistance += road.distance;
      if (selected.length === locations.length - 1) break;
    }
  }
  return { selected, totalDistance };
}

export function runFractionalKnapsack(capacity = 65) {
  const items = [
    { name: 'Water', value: 60, weight: 10 },
    { name: 'Medicine', value: 100, weight: 20 },
    { name: 'Food', value: 120, weight: 30 },
    { name: 'First Aid Kits', value: 30, weight: 10 },
    { name: 'Blankets', value: 80, weight: 40 },
  ].map((item) => ({ ...item, ratio: item.value / item.weight }));
  items.sort((a, b) => b.ratio - a.ratio);
  let remaining = capacity;
  let value = 0;
  const picks: Array<{ name: string; fraction: number; value: number }> = [];
  for (const item of items) {
    if (remaining <= 0) break;
    const fraction = Math.min(1, remaining / item.weight);
    picks.push({ name: item.name, fraction, value: item.value * fraction });
    value += item.value * fraction;
    remaining -= item.weight * fraction;
  }
  return { capacity, value, remaining, picks };
}

export function runCoinChange(amount = 63, coins = [1, 5, 10, 25]) {
  const dp = Array(amount + 1).fill(Infinity);
  dp[0] = 0;
  for (let i = 1; i <= amount; i++) {
    for (const coin of coins) {
      if (coin <= i) dp[i] = Math.min(dp[i], dp[i - coin] + 1);
    }
  }
  return { amount, coins, minimum: Number.isFinite(dp[amount]) ? dp[amount] : null };
}

export function runMaxFlowDemo() {
  return {
    maximumFlow: 150,
    augmentingPaths: [60, 40, 10, 40],
    cut: [
      { from: 'Central Shelter', to: 'Evacuation Complete', capacity: 70 },
      { from: 'School Ground', to: 'Evacuation Complete', capacity: 80 },
    ],
  };
}

export function runBipartiteMatchingDemo() {
  return [
    { emergency: 'E1 - Fire', team: 'T1 - Alpha Team' },
    { emergency: 'E2 - Flood', team: 'T2 - Bravo Team' },
    { emergency: 'E3 - Accident', team: 'T3 - Charlie Team' },
  ];
}

export function runTspDemo() {
  const names = ['Emergency Center', 'Industrial Area', 'City Hospital', 'Market Area'];
  const matrix = [
    [0, 14, 34, 44],
    [14, 0, 20, 34],
    [34, 20, 0, 14],
    [44, 34, 14, 0],
  ];
  const n = names.length;
  const memo = new Map<string, { cost: number; path: number[] }>();
  function solve(mask: number, current: number): { cost: number; path: number[] } {
    const key = `${mask}-${current}`;
    const cached = memo.get(key);
    if (cached) return cached;
    if (mask === (1 << n) - 1) return { cost: matrix[current][0], path: [0] };
    let best = { cost: Infinity, path: [] as number[] };
    for (let next = 1; next < n; next++) {
      if ((mask & (1 << next)) === 0) {
        const rest = solve(mask | (1 << next), next);
        const cost = matrix[current][next] + rest.cost;
        if (cost < best.cost) best = { cost, path: [next, ...rest.path] };
      }
    }
    memo.set(key, best);
    return best;
  }
  const result = solve(1, 0);
  return { names, distance: result.cost, path: [0, ...result.path].map((i) => names[i]) };
}

export function routeForIncident(locationId: string, shelterId: string) {
  return previewRoute(locationId, shelterId, 'distance');
}
