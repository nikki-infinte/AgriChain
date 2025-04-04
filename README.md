# AgriChain
AgriChain shows efficient utilization of Data-Structure LinkedList for traceablility , Queue management for First-In-First-Out Service and priority Queue for Quality  wise Demand supply . Binary Tree for Decision making over multiple criteria such as Demand based supply , area code and goes on .
This application is a comprehensive agricultural supply chain management system that tracks crops from harvest to market using advanced data structures. It implements a traceability chain (linked list) for transaction history and a decision tree for intelligent crop routing based on region, quality, and market demand.

## Features
- *Crop Information Management*: Store and track detailed crop data including type, quantity, quality metrics, certifications, and origin.
- *Traceability Chain*: Maintain a complete transaction history for each crop using a linked list implementation.
- *Intelligent Routing*: Utilize a binary decision tree to route crops based on:
  - Regional origin (North, South, East, West)
  - Quality metrics (premium vs standard)
  - Market demand by region and crop type
- *Queue-based Processing*: Manage crop processing at each node with dedicated queues.
- *Multiple User Roles*:
  - Farmers can enter new crops into the system
  - Traders can process crops and make routing decisions
  - All users can view crop history and system status

### Key Data Structures
1) Linked List
   Represents a transaction in the traceability chain. Each transaction node stores :Transaction ID, timestamp, handler details, action taken, crop details, and linked list pointers.
2) Binary Tree (pointers leftChild and rightChild) → Implements decision-making based on criteria like region and quality.
3) Queue (queue<TransactionNode*> processingQueue) → Holds transactions waiting for processing.
4) Hash Map (unordered_map<string, TransactionNode*> transactionMap) → Stores transactions for quick lookup.
5) Vector (vector<TransactionNode*> allTransactions) → Maintains a list of all transactions. To Manages linked list operations. Fetches the complete history of a crop.Lists all available crops with details.

Test case :
PS G:\Innovation_DSANexus> cd "g:\Innovation_DSANexus\" ; if ($?) { g++ Main.cpp -o Main } ; if ($?) { .\Main }

===== AGRICULTURAL SUPPLY CHAIN =====
1. Farmer: Enter New Crop
2. Trader: Process Crop
3. View Crop History
4. View Queue Status
5. View Binary Tree Structure
6. List All Crops
7. Exit
Choice: 1
Enter crop type: Wheat
Enter quantity (kg): 50
Enter freshness (1-10): 10
Is organic? (1=yes, 0=no): 1
Enter farmer ID: 01
Enter location: Pune
Select area code:
1. North
2. South
3. East
4. West
Choice (1-4): 1

Crop entered successfully!
Crop ID: CROP1001 (save this for tracking)
Destination node: northPremium - North Premium

===== AGRICULTURAL SUPPLY CHAIN =====
1. Farmer: Enter New Crop
2. Trader: Process Crop
3. View Crop History
4. View Queue Status
5. View Binary Tree Structure
6. List All Crops
7. Exit
Choice: 5

===== BINARY TREE STRUCTURE =====
ROOT: root (Region Split: North/South vs East/West)
+--- northSouth (North vs South)
|    +--- north (North: Premium vs Standard)
|    |    +--- northPremium (North Premium) [Queue: 1]
|    |    +--- northStandard (North Standard)
|    +--- south (South: Premium vs Standard)
|    |    +--- southPremium (South Premium)
|    |    +--- southStandard (South Standard)
+--- eastWest (East vs West)
|    +--- east (East: Premium vs Standard)
|    |    +--- eastPremium (East Premium)
|    |    +--- eastStandard (East Standard)
|    +--- west (West: Premium vs Standard)
|    |    +--- westPremium (West Premium)
|    |    +--- westStandard (West Standard)

===== AGRICULTURAL SUPPLY CHAIN =====
1. Farmer: Enter New Crop
2. Trader: Process Crop
3. View Crop History
4. View Queue Status
5. View Binary Tree Structure
6. List All Crops
7. Exit
Choice: 1
Enter crop type: Orange
Enter quantity (kg): 80
Enter freshness (1-10): 5
Is organic? (1=yes, 0=no): 0
Enter farmer ID: 01
Enter location: Pune
Select area code:
1. North
2. South
3. East
4. West
Choice (1-4): 2

Crop entered successfully!
Crop ID: CROP1003 (save this for tracking)
Destination node: southStandard - South Standard

===== AGRICULTURAL SUPPLY CHAIN =====
1. Farmer: Enter New Crop
2. Trader: Process Crop
3. View Crop History
4. View Queue Status
5. View Binary Tree Structure
6. List All Crops
7. Exit
Choice: 5

===== BINARY TREE STRUCTURE =====
ROOT: root (Region Split: North/South vs East/West)
+--- northSouth (North vs South)
|    +--- north (North: Premium vs Standard)
|    |    +--- northPremium (North Premium) [Queue: 1]
|    |    +--- northStandard (North Standard)
|    +--- south (South: Premium vs Standard)
|    |    +--- southPremium (South Premium)
|    |    +--- southStandard (South Standard) [Queue: 1]
+--- eastWest (East vs West)
|    +--- east (East: Premium vs Standard)
|    |    +--- eastPremium (East Premium)
|    |    +--- eastStandard (East Standard)
|    +--- west (West: Premium vs Standard)
|    |    +--- westPremium (West Premium)
|    |    +--- westStandard (West Standard)

===== AGRICULTURAL SUPPLY CHAIN =====
1. Farmer: Enter New Crop
2. Trader: Process Crop
3. View Crop History
4. View Queue Status
5. View Binary Tree Structure
6. List All Crops
7. Exit
Choice: 3

===== AVAILABLE CROPS =====
ID        Type        Quantity    Area      Handler        Current Status
----------------------------------------------------------------------
CROP1003  Orange      80          South     Farmer         Initial harvest ent
CROP1001  Wheat       50          North     Farmer         Initial harvest ent

Enter crop ID to trace: CROP1001

===== CROP HISTORY =====
Transaction 1: TRANS1002
  Time: Fri Apr  4 11:36:03 2025
  Handler: Farmer (01)
  Location: Pune
  Action: Initial harvest entry Regional demand: 8.500000/10 (High) | root decision: left | northSouth decision: left | north decision: left | Final path: root -> northSouth -> north -> northPremium
  Next Destination: Node: northPremium (North Premium)
------------------------

===== AGRICULTURAL SUPPLY CHAIN =====
1. Farmer: Enter New Crop
2. Trader: Process Crop
3. View Crop History
4. View Queue Status
5. View Binary Tree Structure
6. List All Crops
7. Exit
Choice: 2

===== AVAILABLE QUEUES WITH CROPS =====
1. northPremium (North Premium) - Items: 1
2. southStandard (South Standard) - Items: 1
Select node to process (1-2): 1

===== CROP DETAILS =====
Crop ID: CROP1001
Type: Wheat
Quantity: 50 kg
Harvest Date: Fri Apr  4 11:36:03 2025
Farmer ID: 01
Origin: Pune (Area: North)
Quality Metrics:
  - freshness: 10/10
Certifications: Organic

Enter trader ID: 1037
Enter current location: gujrat
Select decision:
1. Route to Manufacturer
2. Route to Retailer
3. Route to Export
Choice: 3

Trader decision processed successfully!
Transaction ID: TRANS1005

===== AGRICULTURAL SUPPLY CHAIN =====
1. Farmer: Enter New Crop
2. Trader: Process Crop
3. View Crop History
4. View Queue Status
5. View Binary Tree Structure
6. List All Crops
7. Exit
Choice: 5

===== BINARY TREE STRUCTURE =====
ROOT: root (Region Split: North/South vs East/West)
+--- northSouth (North vs South)
|    +--- north (North: Premium vs Standard)
|    |    +--- northPremium (North Premium)
|    |    +--- northStandard (North Standard)
|    +--- south (South: Premium vs Standard)
|    |    +--- southPremium (South Premium)
|    |    +--- southStandard (South Standard) [Queue: 1]
+--- eastWest (East vs West)
|    +--- east (East: Premium vs Standard)
|    |    +--- eastPremium (East Premium)
|    |    +--- eastStandard (East Standard)
|    +--- west (West: Premium vs Standard)
|    |    +--- westPremium (West Premium)
|    |    +--- westStandard (West Standard)

===== AGRICULTURAL SUPPLY CHAIN =====
|    |    +--- eastStandard (East Standard)
|    +--- west (West: Premium vs Standard)
|    |    +--- westPremium (West Premium)
|    |    +--- westStandard (West Standard)

===== AGRICULTURAL SUPPLY CHAIN =====
|    +--- west (West: Premium vs Standard)
|    |    +--- westPremium (West Premium)
|    |    +--- westStandard (West Standard)

===== AGRICULTURAL SUPPLY CHAIN =====

===== AGRICULTURAL SUPPLY CHAIN =====
===== AGRICULTURAL SUPPLY CHAIN =====
1. Farmer: Enter New Crop
2. Trader: Process Crop
1. Farmer: Enter New Crop
2. Trader: Process Crop
2. Trader: Process Crop
3. View Crop History
3. View Crop History
4. View Queue Status
5. View Binary Tree Structure
6. List All Crops
7. Exit
Choice: 3

===== AVAILABLE CROPS =====
ID        Type        Quantity    Area      Handler        Current Status
----------------------------------------------------------------------
CROP1003  Orange      80          South     Farmer         Initial harvest ent
CROP1001  Wheat       50          North     Trader         Route to Export

Enter crop ID to trace: No history found for this crop.

===== AGRICULTURAL SUPPLY CHAIN =====
PS G:\Innovation_DSANexus>



