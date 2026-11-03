# Rescue Ops Dashboard

Build a professional web dashboard called:

"Smart Disaster Management & Evacuation Optimization System"

This is a 3rd-year Computer Science DAA (Design and Analysis of Algorithms) project.

IMPORTANT:
This is NOT a generic admin dashboard. It is a disaster-management simulation and algorithm visualization platform.

For the first version, use realistic MOCK DATA only.
Do NOT create the backend or API yet.
The frontend will later be connected to a C++/Python backend containing DAA algorithms.

==================================================
1. TECHNOLOGY
==================================================

Use:

- React
- TypeScript
- Tailwind CSS
- Modern component-based architecture
- Responsive desktop-first design
- Lucide icons or another clean icon library
- Reusable components
- Clean folder structure

The application should run smoothly in the browser.

==================================================
2. VISUAL DESIGN
==================================================

Create a professional emergency operations/control-room interface.

Design style:

- Modern
- Professional
- Clean
- Technical
- Data-driven
- Suitable for a college final-year/third-year project demonstration
- Dark sidebar
- Light main dashboard
- Clear cards and charts
- Subtle borders and shadows
- Rounded corners, but not overly rounded
- Good spacing
- Strong typography hierarchy

Use a restrained emergency-management color system:

- Red = active/critical emergency
- Orange = warning/high severity
- Green = safe/available/open
- Blue = information/algorithm/system data
- Gray = inactive/normal

Do NOT make the entire interface red.

The dashboard should look like a real disaster-management control center.

==================================================
3. APPLICATION LAYOUT
==================================================

Create a persistent left sidebar.

Sidebar title:

"SmartDisaster"

Subtitle:

"DAA Optimization System"

Navigation:

1. Dashboard
2. City Map
3. Emergencies
4. Evacuation
5. Rescue Teams
6. Shelters
7. Resources
8. Algorithms
9. Simulation
10. Reports

At the bottom of sidebar:

System Status
● All Systems Operational

Main content area should have:

- Top header
- Page title
- Current system status
- Notification icon
- User/admin profile
- Main content

==================================================
4. DASHBOARD PAGE
==================================================

Create an impressive overview dashboard.

Header:

"Emergency Operations Dashboard"

Subtitle:

"Real-time disaster monitoring and DAA-based optimization"

At the top show four major KPI cards:

1. Active Emergencies
Value: 3
Status: Requires Attention
Icon: AlertTriangle

2. People Affected
Value: 450
Status: Across 3 incidents
Icon: Users

3. Available Rescue Teams
Value: 4
Status: All available
Icon: Shield

4. Shelter Capacity
Value: 1,100 / 1,500
Status: 73% occupied
Icon: Home

Below these cards create:

--------------------------------------------------
ACTIVE EMERGENCIES
--------------------------------------------------

Show emergency cards/table:

Emergency ID | Type | Location | Severity | Affected People | Required Resource | Status

E1 | FIRE | Industrial Area | CRITICAL | 150 | WATER | ACTIVE
E2 | FLOOD | Market Area | HIGH | 250 | FOOD | ACTIVE
E3 | ACCIDENT | South Junction | HIGH | 50 | MEDICINE | ACTIVE

Use severity badges.

Critical = red
High = orange
Active = appropriate status badge

--------------------------------------------------
CITY OVERVIEW
--------------------------------------------------

Create a large city network visualization.

Show:

- Nodes representing locations
- Roads connecting nodes
- Emergency locations highlighted
- Shelters highlighted
- Hospital highlighted
- Emergency Center highlighted
- Rescue team locations
- Road capacity indicators

Use the following 12 locations:

L1 - Central Junction
L2 - North Junction
L3 - East Junction
L4 - West Junction
L5 - South Junction
L6 - City Hospital
L7 - Central Shelter
L8 - Emergency Center
L9 - North Shelter
L10 - Industrial Area
L11 - School Ground
L12 - Market Area

This does not need to be a real geographical map initially.
Create a clean simulated city graph/network visualization.

--------------------------------------------------
SHELTER CAPACITY
--------------------------------------------------

Show three shelter cards:

Central Shelter
Capacity: 500
Occupied: 120
Available: 380
Status: OPEN

North Shelter
Capacity: 400
Occupied: 80
Available: 320
Status: OPEN

School Ground Shelter
Capacity: 600
Occupied: 200
Available: 400
Status: OPEN

Show progress bars.

--------------------------------------------------
RESOURCE STATUS
--------------------------------------------------

Show:

Water: 1,800 L
Food: 1,200 packets
Medicine: 300 packets
Blankets: 400 units

Use clean resource cards.

--------------------------------------------------
ALGORITHM STATUS
--------------------------------------------------

Show the DAA algorithms currently available:

BFS          READY
DFS          READY
Dijkstra     READY
MST          READY
Dynamic Programming READY
Greedy       READY
Backtracking READY
Max Flow     READY

These are currently mock statuses.

==================================================
5. CITY MAP PAGE
==================================================

Create a dedicated "City Map & Network" page.

Display a large interactive simulated graph.

Locations should be displayed as nodes.

Roads should show:

- Distance
- Travel time
- Capacity
- Status

Example:

L1 → L2
Distance: 10 km
Time: 5 min
Capacity: 100

Allow the user to click a location and show its information in a side panel.

Location panel should show:

Location ID
Name
Type
Connected Roads
Emergency Status
Nearby Shelters
Nearby Rescue Teams

Add filters:

All
Junctions
Hospitals
Shelters
Emergency Centers
Emergency Areas

==================================================
6. EMERGENCIES PAGE
==================================================

Create an emergency management page.

Show:

- Total emergencies
- Critical
- High
- Resolved

Create a table containing:

Emergency ID
Type
Location
Severity
Affected People
Required Resource
Status
Actions

Add button:

"+ Report Emergency"

Emergency details page/modal should contain:

Emergency information
Affected people
Required resources
Nearest rescue team
Nearest hospital
Nearest shelter
Recommended route

Initially use mock calculations.

==================================================
7. EVACUATION PAGE
==================================================

Create a DAA-focused evacuation optimization interface.

Title:

"Evacuation Route Optimization"

Controls:

Emergency Location
Destination Shelter
Optimization Criteria

Optimization criteria:

Shortest Distance
Minimum Time
Maximum Capacity

Button:

"Calculate Optimal Route"

After clicking, display:

Recommended Route
Total Distance
Estimated Time
Road Capacity
Number of Nodes
Algorithm Used

Example:

Algorithm:
Dijkstra's Shortest Path

Route:

Industrial Area
↓
East Junction
↓
Central Junction
↓
North Junction
↓
Central Shelter

Distance:
30 km

Estimated Time:
15 minutes

Also show alternative routes.

IMPORTANT:
Clearly label this as simulated/mock output for now.

==================================================
8. RESCUE TEAMS PAGE
==================================================

Create rescue team management.

Teams:

T1 - Alpha Team
Type: FIRE
Location: Central Junction
Capacity: 50
Status: AVAILABLE

T2 - Bravo Team
Type: FLOOD
Location: Central Shelter
Capacity: 100
Status: AVAILABLE

T3 - Charlie Team
Type: MEDICAL
Location: City Hospital
Capacity: 30
Status: AVAILABLE

T4 - Delta Team
Type: GENERAL
Location: Emergency Center
Capacity: 70
Status: AVAILABLE

Show team cards and table.

Include:

Team Type
Current Location
Capacity
Availability
Assigned Emergency

==================================================
9. SHELTERS PAGE
==================================================

Create shelter management.

Shelters:

S1 - Central Shelter
Location: L7
Capacity: 500
Occupied: 120
Available: 380

S2 - North Shelter
Location: L9
Capacity: 400
Occupied: 80
Available: 320

S3 - School Ground Shelter
Location: L11
Capacity: 600
Occupied: 200
Available: 400

Show:

- Capacity
- Occupancy
- Available spaces
- Occupancy percentage
- Status

==================================================
10. RESOURCES PAGE
==================================================

Create resource management.

Resources:

RES1 - WATER
Location: Central Junction
Quantity: 1000 LITERS
Priority: 10

RES2 - FOOD
Location: Central Shelter
Quantity: 500 PACKETS
Priority: 8

RES3 - MEDICINE
Location: City Hospital
Quantity: 300 PACKETS
Priority: 10

RES4 - BLANKET
Location: North Shelter
Quantity: 400 UNITS
Priority: 6

RES5 - FOOD
Location: School Ground
Quantity: 700 PACKETS
Priority: 8

RES6 - WATER
Location: Emergency Center
Quantity: 800 LITERS
Priority: 9

Create resource cards and table.

==================================================
11. ALGORITHMS PAGE
==================================================

This is a VERY IMPORTANT page because this is a DAA project.

Create an "Algorithm Laboratory".

Show algorithm cards.

Categories:

UNIT I
- Searching
- Sorting
- BFS
- DFS
- Topological Sort

UNIT II
- Recursion
- Merge Sort
- Quick Sort
- Closest Pair
- Majority Element
- Order Statistics
- Euclid
- Karatsuba
- Randomized Algorithms

UNIT III
- Fibonacci
- Binomial
- Matrix Chain Multiplication
- Coin Change
- TSP
- Optimal BST
- DAG Shortest Path
- Floyd-Warshall
- LIS
- LCS
- Independent Set

UNIT IV
- Dijkstra
- MST
- Fractional Knapsack
- Scheduling
- Huffman
- Backtracking

UNIT V
- Ford-Fulkerson
- Max Flow
- Min Cut
- Bipartite Matching

UNIT VI
- SAT
- 3-SAT
- Clique
- Vertex Cover
- ILP

Each algorithm card should show:

Algorithm Name
Unit
Problem Solved
Real-world Application
Time Complexity
Space Complexity
Status

Example:

DIJKSTRA

Problem:
Find the shortest path between locations.

Application:
Emergency evacuation route optimization.

Time:
O((V + E) log V)

Status:
Ready

Add button:

"View Algorithm"

==================================================
12. SIMULATION PAGE
==================================================

Create a disaster simulation interface.

Title:

"Disaster Simulation"

Allow user to select:

Disaster Type:
Fire
Flood
Accident

Emergency Location:
Industrial Area
Market Area
South Junction

Severity:
Low
Medium
High
Critical

Affected People:

[ input ]

Then button:

"Run Simulation"

After simulation show:

Emergency Summary
Affected Area
Recommended Rescue Team
Recommended Shelter
Optimal Route
Required Resources
Evacuation Flow
Algorithm Used

Create a visual step-by-step result:

1. Detect Emergency
2. Analyze City Graph
3. Find Optimal Route
4. Assign Rescue Team
5. Allocate Resources
6. Calculate Evacuation Flow
7. Recommend Shelter

==================================================
13. REPORTS PAGE
==================================================

Create reports and analytics.

Show:

- Emergency history
- Evacuation performance
- Algorithm execution statistics
- Resource usage
- Shelter occupancy
- Rescue team utilization

Include charts:

- Emergencies over time
- Shelter occupancy
- Resource consumption
- Algorithm execution time comparison

Use mock data.

==================================================
14. IMPORTANT CITY DATA
==================================================

Use exactly these city locations:

L1 Central Junction
L2 North Junction
L3 East Junction
L4 West Junction
L5 South Junction
L6 City Hospital
L7 Central Shelter
L8 Emergency Center
L9 North Shelter
L10 Industrial Area
L11 School Ground
L12 Market Area

Roads:

L1-L2: 10 km, 5 min, capacity 100
L1-L3: 10 km, 5 min, capacity 120
L1-L4: 10 km, 5 min, capacity 80
L1-L5: 10 km, 5 min, capacity 100
L2-L12: 14 km, 7 min, capacity 80
L2-L7: 10 km, 5 min, capacity 70
L3-L10: 10 km, 5 min, capacity 100
L3-L6: 14 km, 7 min, capacity 90
L5-L8: 10 km, 5 min, capacity 100
L4-L7: 10 km, 5 min, capacity 60
L12-L9: 10 km, 5 min, capacity 70
L12-L6: 14 km, 7 min, capacity 80
L10-L11: 10 km, 5 min, capacity 100
L6-L11: 10 km, 5 min, capacity 90
L8-L10: 14 km, 7 min, capacity 80

Treat these roads as bidirectional in the initial simulation.

==================================================
15. IMPORTANT IMPLEMENTATION RULES
==================================================

For now:

- Use mock/static data.
- Do not build backend APIs.
- Do not create authentication yet.
- Do not use real maps or external map APIs.
- Do not add unnecessary features.
- Keep the UI modular so backend APIs can be connected later.
- Store mock data in separate TypeScript data files.
- Create reusable cards, tables, badges, dialogs, charts and graph components.
- Make all navigation links functional.
- Use realistic empty/loading/error states.
- Make buttons visually functional even if they currently use mock calculations.

The frontend must be designed so that later we can replace:

MOCK DATA
with
REAL API DATA

without redesigning the interface.

==================================================
16. PROJECT PURPOSE
==================================================

The main purpose of this project is to demonstrate how Design and Analysis of Algorithms can solve disaster-management problems.

The application should therefore clearly show:

CITY GRAPH
↓
ALGORITHM
↓
CALCULATION
↓
RESULT
↓
DISASTER MANAGEMENT DECISION

The interface should make it easy for a college examiner to understand where DAA algorithms are being used.

==================================================
17. FINAL UI QUALITY
==================================================

Make the dashboard polished enough for:

- College project demonstration
- Mid-semester review
- Final project presentation
- DAA viva
- Screenshots in project documentation

Prioritize clarity and usability over excessive animations.

Use subtle animations only for:
- page transitions
- cards
- graph interactions
- status changes

Do not make the interface look like a gaming dashboard.

The final result should feel like a professional emergency operations and algorithm optimization platform.

This project was built with [Lovable](https://lovable.dev).

## Build with Lovable

Continue developing this project in the [Lovable editor](https://lovable.dev/projects/3bc48ce0-0ace-4f4d-83b2-a4277ab23c40).

- **Ship faster**: describe what you want to build and Lovable handles the code.
- **Stay in sync**: every change made in Lovable is committed straight to this repository.
- **Full ownership**: this code is yours. Push to `main` on GitHub and your changes sync back into Lovable, ready for your next prompt.

## Development

Prefer working locally? You need Node.js and npm — [install with nvm](https://github.com/nvm-sh/nvm#installing-and-updating).

```sh
git clone <this-repository-url>
cd <repository-name>
npm i
npm run dev
```
