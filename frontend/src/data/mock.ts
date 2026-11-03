export type LocationType = 'Junction' | 'Hospital' | 'Shelter' | 'Emergency Center' | 'Emergency Area';
export type Location = { id: string; name: string; type: LocationType; x: number; y: number };
export const locations: Location[] = [
  {id:'L1',name:'Central Junction',type:'Junction',x:48,y:48},
  {id:'L2',name:'North Junction',type:'Junction',x:48,y:23},
  {id:'L3',name:'East Junction',type:'Junction',x:70,y:48},
  {id:'L4',name:'West Junction',type:'Junction',x:26,y:48},
  {id:'L5',name:'South Junction',type:'Emergency Area',x:48,y:76},
  {id:'L6',name:'City Hospital',type:'Hospital',x:87,y:29},
  {id:'L7',name:'Central Shelter',type:'Shelter',x:24,y:23},
  {id:'L8',name:'Emergency Center',type:'Emergency Center',x:68,y:85},
  {id:'L9',name:'North Shelter',type:'Shelter',x:67,y:8},
  {id:'L10',name:'Industrial Area',type:'Emergency Area',x:88,y:62},
  {id:'L11',name:'School Ground',type:'Shelter',x:91,y:85},
  {id:'L12',name:'Market Area',type:'Emergency Area',x:67,y:23},
];
export type Road = { from: string; to: string; distance: number; time: number; capacity: number; status: 'Open' };
export const roads: Road[] = [
 ['L1','L2',10,5,100],['L1','L3',10,5,120],['L1','L4',10,5,80],['L1','L5',10,5,100],
 ['L2','L12',14,7,80],['L2','L7',10,5,70],['L3','L10',10,5,100],['L3','L6',14,7,90],
 ['L5','L8',10,5,100],['L4','L7',10,5,60],['L12','L9',10,5,70],['L12','L6',14,7,80],
 ['L10','L11',10,5,100],['L6','L11',10,5,90],['L8','L10',14,7,80]
].map(([from,to,distance,time,capacity]) => ({from:String(from),to:String(to),distance:Number(distance),time:Number(time),capacity:Number(capacity),status:'Open' as const}));
export const emergencies = [
 {id:'E1',type:'Fire',location:'Industrial Area',locationId:'L10',severity:'Critical',affected:150,resource:'Water',status:'Active',team:'Alpha Team',hospital:'City Hospital',shelter:'School Ground Shelter'},
 {id:'E2',type:'Flood',location:'Market Area',locationId:'L12',severity:'High',affected:250,resource:'Food',status:'Active',team:'Bravo Team',hospital:'City Hospital',shelter:'North Shelter'},
 {id:'E3',type:'Accident',location:'South Junction',locationId:'L5',severity:'High',affected:50,resource:'Medicine',status:'Active',team:'Charlie Team',hospital:'City Hospital',shelter:'Central Shelter'},
];
export type Emergency = typeof emergencies[number];
export const shelters = [
 {id:'S1',name:'Central Shelter',locationId:'L7',capacity:500,occupied:120},
 {id:'S2',name:'North Shelter',locationId:'L9',capacity:400,occupied:80},
 {id:'S3',name:'School Ground Shelter',locationId:'L11',capacity:600,occupied:200},
];
export const teams = [
 {id:'T1',name:'Alpha Team',type:'Fire',location:'Central Junction',locationId:'L1',capacity:50,status:'Available',assignment:'Unassigned'},
 {id:'T2',name:'Bravo Team',type:'Flood',location:'Central Shelter',locationId:'L7',capacity:100,status:'Available',assignment:'Unassigned'},
 {id:'T3',name:'Charlie Team',type:'Medical',location:'City Hospital',locationId:'L6',capacity:30,status:'Available',assignment:'Unassigned'},
 {id:'T4',name:'Delta Team',type:'General',location:'Emergency Center',locationId:'L8',capacity:70,status:'Available',assignment:'Unassigned'},
];
export const resources = [
 {id:'RES1',name:'Water',location:'Central Junction',quantity:1000,unit:'L',priority:10},
 {id:'RES2',name:'Food',location:'Central Shelter',quantity:500,unit:'packets',priority:8},
 {id:'RES3',name:'Medicine',location:'City Hospital',quantity:300,unit:'packets',priority:10},
 {id:'RES4',name:'Blankets',location:'North Shelter',quantity:400,unit:'units',priority:6},
 {id:'RES5',name:'Food',location:'School Ground',quantity:700,unit:'packets',priority:8},
 {id:'RES6',name:'Water',location:'Emergency Center',quantity:800,unit:'L',priority:9},
];
export const resourceTotals = [{name:'Water',value:'1,800 L'},{name:'Food',value:'1,200 packets'},{name:'Medicine',value:'300 packets'},{name:'Blankets',value:'400 units'}];
export type Algorithm = {name:string; unit:string; problem:string; application:string; time:string; space:string; status:string};
const algorithmGroups: Record<string,string[]> = {
  'UNIT I':['Searching','Sorting','BFS','DFS','Topological Sort'],
  'UNIT II':['Recursion','Merge Sort','Quick Sort','Closest Pair','Majority Element','Order Statistics','Euclid','Karatsuba','Fast Modular Exponentiation','Josephus','Tower of Hanoi','Inversion Count','Randomized Algorithms'],
  'UNIT III':['Fibonacci','Binomial Coefficient','Matrix Chain Multiplication','Coin Change','TSP','Optimal BST','DAG Shortest Path','Floyd-Warshall','LIS','LCS','Independent Set'],
  'UNIT IV':['Dijkstra','MST','Fractional Knapsack','Scheduling','Huffman','N-Queens','Graph Coloring','Hamiltonian Cycle','Subset Sum','Sudoku'],
  'UNIT V':['Ford-Fulkerson','Max Flow','Min Cut','Bipartite Matching'],
  'UNIT VI':['SAT','Circuit-SAT','3-SAT','Clique','Vertex Cover','ILP'],
};
const algorithmInfo: Record<string,[string,string,string,string]> = {
  'Searching':['Locate a target item in a collection.','Find disaster locations or incident records quickly.','O(n) worst','O(1)'],
  'Sorting':['Arrange records according to a key.','Prioritize emergencies by affected people or severity.','O(n log n) representative','O(n)'],
  'BFS':['Traverse a network level by level.','Find reachable shelters and response zones.','O(V + E)','O(V)'],
  'DFS':['Explore all reachable graph branches.','Inspect road-network connectivity and components.','O(V + E)','O(V)'],
  'Topological Sort':['Order dependencies in a DAG.','Sequence emergency response tasks.','O(V + E)','O(V)'],
  'Recursion':['Solve a problem through smaller self-similar calls.','Expand response hierarchies and recursive planning tasks.','Varies','O(n) stack'],
  'Merge Sort':['Sort records by divide and conquer.','Order emergency records by affected population.','O(n log n)','O(n)'],
  'Quick Sort':['Partition records around a pivot.','Fast in-memory priority sorting.','O(n log n) avg; O(n²) worst','O(log n) avg'],
  'Closest Pair':['Find the nearest two points.','Identify nearby rescue locations.','O(n log n)','O(n)'],
  'Majority Element':['Find an element occurring more than half the time.','Detect dominant incident/resource type.','O(n)','O(1)'],
  'Order Statistics':['Find the kth smallest/largest item.','Identify the kth most affected emergency.','O(n) avg; O(n²) worst','O(1) avg'],
  'Euclid':['Compute greatest common divisor efficiently.','Normalize resource package sizes.','O(log min(a,b))','O(log n) recursive'],
  'Karatsuba':['Multiply large integers faster than grade-school multiplication.','Demonstrate fast arithmetic in the algorithm lab.','O(n^log2(3))','O(n)'],
  'Fast Modular Exponentiation':['Compute large powers modulo m efficiently.','Support efficient modular computations.','O(log exponent)','O(1) iterative'],
  'Josephus':['Determine the survivor in circular elimination.','Model rotating team-selection scenarios.','O(n)','O(n) recursive'],
  'Tower of Hanoi':['Move disks under ordering constraints.','Demonstrate recursive state search.','O(2^n)','O(n)'],
  'Inversion Count':['Count out-of-order pairs.','Measure disorder in emergency priority sequences.','O(n log n)','O(n)'],
  'Randomized Algorithms':['Use randomization to improve expected behavior.','Demonstrate randomized sorting strategies.','O(n log n) expected','O(log n) expected'],
  'Fibonacci':['Build overlapping subproblems with DP.','Demonstrate staged response planning.','O(n)','O(n)'],
  'Binomial Coefficient':['Compute combinations with overlapping subproblems.','Count possible team selections.','O(nk)','O(nk)'],
  'Matrix Chain Multiplication':['Choose the cheapest matrix multiplication order.','Optimize chains of analytical transformations.','O(n³)','O(n²)'],
  'Coin Change':['Minimize the number of denominations for an amount.','Assemble resource packages efficiently.','O(amount × coins)','O(amount)'],
  'TSP':['Visit each selected location once with minimum tour cost.','Plan a multi-stop rescue tour.','O(n²2^n) DP','O(n2^n)'],
  'Optimal BST':['Minimize expected search cost in a BST.','Prioritize frequently searched locations.','O(n³)','O(n²)'],
  'DAG Shortest Path':['Find shortest paths in a DAG.','Evaluate ordered response stages.','O(V + E)','O(V)'],
  'Floyd-Warshall':['Find shortest paths between every pair.','Compare routes across all city locations.','O(V³)','O(V²)'],
  'LIS':['Find the longest increasing subsequence.','Analyze rising emergency-priority trends.','O(n²) DP','O(n)'],
  'LCS':['Find the longest common subsequence.','Compare common response steps between plans.','O(nm)','O(nm)'],
  'Independent Set':['Select maximum pairwise non-adjacent tree nodes.','Choose non-conflicting response positions.','O(n)','O(n)'],
  'MST':['Connect all nodes at minimum total cost.','Plan a low-cost emergency road network.','O(E log V)','O(V + E)'],
  'Fractional Knapsack':['Maximize value under a capacity limit with divisible items.','Prioritize loading aid supplies.','O(n log n)','O(n)'],
  'Scheduling':['Order jobs to maximize priority under deadlines.','Dispatch emergency operations efficiently.','O(n log n)','O(n)'],
'Huffman':['Build optimal prefix codes from symbol frequencies.','Compress disaster reports for storage.','O(n + k log k)','O(n + k)'],
  'N-Queens':['Place queens without row, column or diagonal conflicts.','Demonstrate constraint satisfaction.','O(n!)','O(n)'],
  'Graph Coloring':['Assign colors so adjacent vertices differ.','Model conflict-free allocation on a road graph.','O(m^V)','O(V)'],
  'Hamiltonian Cycle':['Visit each vertex exactly once and return to start.','Test complete emergency tours on small graphs.','O(V!)','O(V)'],
  'Subset Sum':['Determine whether a subset reaches an exact target.','Check whether resource quantities can meet a target.','O(2^n)','O(n)'],
  'Sudoku':['Fill a grid while respecting row, column and box constraints.','Demonstrate backtracking constraint solving.','O(9^m)','O(m)'],
  'Dijkstra':['Find shortest paths in a non-negative weighted graph.','Optimize emergency evacuation routes.','O((V + E) log V)','O(V + E)'],
 'Ford-Fulkerson':['Find augmenting paths in a flow network.','Allocate evacuation flow across connections.','O(V * E^2) Edmonds-Karp','O(V^2) residual matrix'],
  'Max Flow':['Find the maximum feasible flow from source to sink.','Estimate evacuation throughput.','O(VE²) Edmonds-Karp','O(V²)'],
  'Min Cut':['Find the minimum-capacity source-sink cut.','Identify evacuation bottlenecks.','O(VE²) via max flow','O(V²)'],
  'Bipartite Matching':['Match two disjoint sets without conflicts.','Match rescue teams to incidents.','O(VE)','O(V)'],
 'SAT':['Determine whether a Boolean formula has a satisfying assignment.','Model binary disaster-resource decisions.','O(2^n) brute force demo','O(1) auxiliary space (3-variable demo)'],
'Circuit-SAT':['Determine whether a Boolean circuit can output TRUE.','Demonstrate circuit feasibility.','O(2^n) brute force demo','O(1) auxiliary space (3-input demo)'],
'3-SAT':['Determine satisfiability of a 3-CNF formula.','Demonstrate an NP-complete decision problem.','O(2^n) brute force demo','O(1) auxiliary space (3-variable demo)'],
'Clique':['Determine whether k pairwise-adjacent vertices exist.','Find tightly connected response clusters.','O(2^V * V^2) backtracking upper bound','O(V)'],
  'Vertex Cover':['Select vertices that cover every edge.','Place resources to cover connected roads.','O(2^V * E * V) backtracking upper bound','O(V)'],
  'ILP':['Optimize linear objectives with integer decision variables.','Model discrete disaster-planning decisions.','O(2^V * (V + E)) brute-force enumeration','O(V) auxiliary space'],
};
export const algorithms: Algorithm[] = Object.entries(algorithmGroups).flatMap(([unit,names]) => names.map(name => {
  const info = algorithmInfo[name] ?? ['Solve a computational optimization problem.','Explore planning and decision-making scenarios.','Varies by approach','Varies by approach'];
  return {name,unit,problem:info[0],application:info[1],time:info[2],space:info[3],status:'Ready'};
}));
export const activeAlgorithms = ['BFS','DFS','Dijkstra','MST','Dynamic Programming','Greedy','Backtracking','Max Flow'];

// Local, deterministic preview calculation over the static bidirectional graph.
// This is not connected to an external optimization engine.
export function previewRoute(start:string,end:string,criteria:'distance'|'time'|'capacity'='distance', excluded:string[]=[]){
 const dist:Record<string,number> = Object.fromEntries(locations.map(l=>[l.id,Infinity]));
 const previous:Record<string,string|undefined> = {};
 const visited = new Set<string>();
 dist[start]=0;
 while(visited.size<locations.length){
  const current=locations.filter(l=>!visited.has(l.id)).sort((a,b)=>(dist[a.id] ?? Infinity)-(dist[b.id] ?? Infinity))[0];
  if(!current || !Number.isFinite(dist[current.id]) || current.id===end) break;
  visited.add(current.id);
  for(const road of roads){
   if(excluded.includes(`${road.from}-${road.to}`)) continue;
   const neighbor=road.from===current.id?road.to:road.to===current.id?road.from:null;
   if(!neighbor || visited.has(neighbor)) continue;
   const weight=criteria==='time'?road.time:criteria==='capacity'?1000/road.capacity:road.distance;
   if((dist[current.id] ?? Infinity)+weight<(dist[neighbor] ?? Infinity)){dist[neighbor]=(dist[current.id] ?? Infinity)+weight;previous[neighbor]=current.id;}
  }
 }
 if(!Number.isFinite(dist[end])) return null;
 const ids=[end];
 while(ids[0]!==start){const prev=previous[ids[0] ?? ''];if(!prev)return null;ids.unshift(prev)}
 const edges=ids.slice(1).map((id,i)=>roads.find(r=>(r.from===ids[i]&&r.to===id)||(r.to===ids[i]&&r.from===id))).filter((r):r is Road=>Boolean(r));
 return {ids,names:ids.map(id=>locations.find(l=>l.id===id)?.name ?? id),distance:edges.reduce((n,r)=>n+r.distance,0),time:edges.reduce((n,r)=>n+r.time,0),capacity:Math.min(...edges.map(r=>r.capacity))};
}
