#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <ctime>
#include <functional>
#include <iomanip>
using namespace std;

// Crop information structure
struct Crop {
    string id;                          // Unique identifier
    string type;                        // e.g., "Tomato", "Wheat", etc.
    double quantity;                    // Amount in kg
    time_t harvestDate;                 // When it was harvested
    map<string, float> qualityMetrics;  // e.g., {"freshness": 9.5, "size": 8.0}
    vector<string> certifications;      // e.g., "Organic", "Fair Trade" 
    string farmerId;                    // ID of the farmer
    string originLocation;              // Where it was grown
    string areaCode;                    // Region code (North, South, East, West)
    
    // Display crop details
    void display() const {
        cout << "Crop ID: " << id << endl;
        cout << "Type: " << type << endl;
        cout << "Quantity: " << quantity << " kg" << endl;
        cout << "Harvest Date: " << ctime(&harvestDate);
        cout << "Farmer ID: " << farmerId << endl;
        cout << "Origin: " << originLocation << " (Area: " << areaCode << ")" << endl;
        cout << "Quality Metrics:" << endl;
        for (const auto& metric : qualityMetrics) {
            cout << "  - " << metric.first << ": " << metric.second << "/10" << endl;
        }
        if (!certifications.empty()) {
            cout << "Certifications: ";
            for (const auto& cert : certifications) {
                cout << cert << " ";
            }
            cout << endl;
        }
    }
};

// Transaction node for our linked list (traceability chain)
struct TransactionNode {
    string transactionId;
    time_t timestamp;
    string handlerId;                   // Who handled the crop
    string handlerType;                 // "Farmer", "Trader", "Manufacturer", etc.
    string location;
    string actionTaken;                 // What was done with the crop
    string nextDestination;             // Where it's going next
    Crop cropDetails;                   // Copy of crop at this stage
    
    // Linked list pointers
    TransactionNode* previous;
    TransactionNode* next;
    
    // Constructor
    TransactionNode(string id, string handler, string type, 
                   string loc, string action, Crop crop) : 
        transactionId(id), handlerId(handler), handlerType(type),
        location(loc), actionTaken(action), cropDetails(crop),
        timestamp(time(nullptr)), previous(nullptr), next(nullptr) {}
};


struct DecisionNode {
    string nodeId;
    string criteriaType;                // Decision criteria
    string description;                 // Human-readable description
    function<bool(const Crop&)> decisionFunction;  // Decision logic
    queue<TransactionNode*> processingQueue;      // Queue at this node
    
    DecisionNode* leftChild;            // True decision path
    DecisionNode* rightChild;           // False decision path
    
    // Constructor
    DecisionNode(string id, string criteria, string desc) : 
        nodeId(id), criteriaType(criteria), description(desc),
        leftChild(nullptr), rightChild(nullptr) {}
    
    // Enqueue a transaction to this node's queue
    void enqueue(TransactionNode* transaction) {
        processingQueue.push(transaction);
    }
    
    // Get next transaction from queue
    TransactionNode* dequeue() {
        if (processingQueue.empty()) {
            return nullptr;
        }
        
        TransactionNode* transaction = processingQueue.front();
        processingQueue.pop();
        return transaction;
    }
    
    // Check if queue is empty
    bool isQueueEmpty() {
        return processingQueue.empty();
    }
    
    // Get queue size
    int queueSize() {
        return processingQueue.size();
    }
};

// TraceabilityChain - Our linked list implementation
class TraceabilityChain {
private:
    unordered_map<string, TransactionNode*> transactionMap; // For quick lookup
    vector<TransactionNode*> allTransactions; // Store all transactions for listing
    
public:
    // Add new transaction to the chain
    void addTransaction(TransactionNode* node, TransactionNode* previous = nullptr) {
        if (previous != nullptr) {
            previous->next = node;
            node->previous = previous;
        }
        transactionMap[node->transactionId] = node;
        allTransactions.push_back(node);
    }
    
    // Get complete history of a crop
    vector<TransactionNode*> getHistory(string cropId) {
        vector<TransactionNode*> history;
        
        // Find the most recent transaction for this crop
        TransactionNode* current = nullptr;
        for (auto& pair : transactionMap) {
            if (pair.second->cropDetails.id == cropId) {
                if (current == nullptr || pair.second->timestamp > current->timestamp) {
                    current = pair.second;
                }
            }
        }
        
        // If found, trace back to origin
        if (current != nullptr) {
            // Go to origin first
            while (current->previous != nullptr) {
                current = current->previous;
            }
            
            // Now collect all transactions from origin forward
            while (current != nullptr) {
                history.push_back(current);
                current = current->next;
            }
        }
        
        return history;
    }
    
    // List all available crops with their IDs
    void listAllCrops() {
        unordered_map<string, TransactionNode*> latestCropTransactions;
        
        // Find the latest transaction for each crop
        for (auto* transaction : allTransactions) {
            string cropId = transaction->cropDetails.id;
            
            if (latestCropTransactions.find(cropId) == latestCropTransactions.end() ||
                transaction->timestamp > latestCropTransactions[cropId]->timestamp) {
                latestCropTransactions[cropId] = transaction;
            }
        }
        
        // Display crop information
        cout << "\n===== AVAILABLE CROPS =====" << endl;
        cout << left << setw(10) << "ID" 
             << setw(12) << "Type" 
             << setw(12) << "Quantity" 
             << setw(10) << "Area" 
             << setw(15) << "Handler" 
             << setw(20) << "Current Status" << endl;
        cout << string(70, '-') << endl;
        
        for (const auto& pair : latestCropTransactions) {
            Crop crop = pair.second->cropDetails;
            cout << left << setw(10) << crop.id 
                 << setw(12) << crop.type 
                 << setw(12) << crop.quantity 
                 << setw(10) << crop.areaCode 
                 << setw(15) << pair.second->handlerType 
                 << setw(20) << pair.second->actionTaken.substr(0, 19) << endl;
        }
    }
};


class RoutingDecisionTree {
private:
    DecisionNode* root;
    unordered_map<string, DecisionNode*> nodeMap; // Map for quick node lookup
    
    // Market demand data by region
    unordered_map<string, unordered_map<string, float>> regionalDemand;
    
public:
    RoutingDecisionTree() {
        // Initialize regional demand data
        setupRegionalDemand();
        
        // Initialize the decision tree
        setupDecisionTree();
    }
    
    // Set up regional demand data
    void setupRegionalDemand() {
        // North region demand
        regionalDemand["North"]["Wheat"] = 8.5;
        regionalDemand["North"]["Rice"] = 7.0;
        regionalDemand["North"]["Corn"] = 6.0;
        regionalDemand["North"]["Tomato"] = 5.0;
        regionalDemand["North"]["Apple"] = 9.0;
        
        // South region demand
        regionalDemand["South"]["Wheat"] = 5.0;
        regionalDemand["South"]["Rice"] = 9.0;
        regionalDemand["South"]["Corn"] = 6.5;
        regionalDemand["South"]["Tomato"] = 8.0;
        regionalDemand["South"]["Apple"] = 4.0;
        
        // East region demand
        regionalDemand["East"]["Wheat"] = 6.0;
        regionalDemand["East"]["Rice"] = 8.5;
        regionalDemand["East"]["Corn"] = 5.0;
        regionalDemand["East"]["Tomato"] = 7.5;
        regionalDemand["East"]["Apple"] = 6.5;
        
        // West region demand
        regionalDemand["West"]["Wheat"] = 7.0;
        regionalDemand["West"]["Rice"] = 6.0;
        regionalDemand["West"]["Corn"] = 8.0;
        regionalDemand["West"]["Tomato"] = 9.0;
        regionalDemand["West"]["Apple"] = 7.5;
    }
    
    // Set up the initial decision tree structure based on regions and quality
    void setupDecisionTree() {
        // Root: Decision based on region (area)
        root = new DecisionNode("root", "AreaBased", "Region Split: North/South vs East/West");
        root->decisionFunction = [](const Crop& crop) {
            return crop.areaCode == "North" || crop.areaCode == "South";
        };
        nodeMap["root"] = root;
        
        // North-South branch
        root->leftChild = new DecisionNode("northSouth", "AreaBased", "North vs South");
        root->leftChild->decisionFunction = [](const Crop& crop) {
            return crop.areaCode == "North";
        };
        nodeMap["northSouth"] = root->leftChild;
        
        // East-West branch
        root->rightChild = new DecisionNode("eastWest", "AreaBased", "East vs West");
        root->rightChild->decisionFunction = [](const Crop& crop) {
            return crop.areaCode == "East";
        };
        nodeMap["eastWest"] = root->rightChild;
        
        // North region branch - split by quality
        root->leftChild->leftChild = new DecisionNode("north", "QualityBased", "North: Premium vs Standard");
        root->leftChild->leftChild->decisionFunction = [](const Crop& crop) {
            float freshness = crop.qualityMetrics.at("freshness");
            return freshness >= 8.0 || !crop.certifications.empty();
        };
        nodeMap["north"] = root->leftChild->leftChild;
        
        // South region branch - split by quality
        root->leftChild->rightChild = new DecisionNode("south", "QualityBased", "South: Premium vs Standard");
        root->leftChild->rightChild->decisionFunction = [](const Crop& crop) {
            float freshness = crop.qualityMetrics.at("freshness");
            return freshness >= 8.0 || !crop.certifications.empty();
        };
        nodeMap["south"] = root->leftChild->rightChild;
        
        // East region branch - split by quality
        root->rightChild->leftChild = new DecisionNode("east", "QualityBased", "East: Premium vs Standard");
        root->rightChild->leftChild->decisionFunction = [](const Crop& crop) {
            float freshness = crop.qualityMetrics.at("freshness");
            return freshness >= 8.0 || !crop.certifications.empty();
        };
        nodeMap["east"] = root->rightChild->leftChild;
        
        // West region branch - split by quality
        root->rightChild->rightChild = new DecisionNode("west", "QualityBased", "West: Premium vs Standard");
        root->rightChild->rightChild->decisionFunction = [](const Crop& crop) {
            float freshness = crop.qualityMetrics.at("freshness");
            return freshness >= 8.0 || !crop.certifications.empty();
        };
        nodeMap["west"] = root->rightChild->rightChild;
        
        // Leaf nodes for North region
        root->leftChild->leftChild->leftChild = new DecisionNode("northPremium", "FinalDestination", "North Premium");
        nodeMap["northPremium"] = root->leftChild->leftChild->leftChild;
        
        root->leftChild->leftChild->rightChild = new DecisionNode("northStandard", "FinalDestination", "North Standard");
        nodeMap["northStandard"] = root->leftChild->leftChild->rightChild;
        
        // Leaf nodes for South region
        root->leftChild->rightChild->leftChild = new DecisionNode("southPremium", "FinalDestination", "South Premium");
        nodeMap["southPremium"] = root->leftChild->rightChild->leftChild;
        
        root->leftChild->rightChild->rightChild = new DecisionNode("southStandard", "FinalDestination", "South Standard");
        nodeMap["southStandard"] = root->leftChild->rightChild->rightChild;
        
        // Leaf nodes for East region
        root->rightChild->leftChild->leftChild = new DecisionNode("eastPremium", "FinalDestination", "East Premium");
        nodeMap["eastPremium"] = root->rightChild->leftChild->leftChild;
        
        root->rightChild->leftChild->rightChild = new DecisionNode("eastStandard", "FinalDestination", "East Standard");
        nodeMap["eastStandard"] = root->rightChild->leftChild->rightChild;
        
        // Leaf nodes for West region
        root->rightChild->rightChild->leftChild = new DecisionNode("westPremium", "FinalDestination", "West Premium");
        nodeMap["westPremium"] = root->rightChild->rightChild->leftChild;
        
        root->rightChild->rightChild->rightChild = new DecisionNode("westStandard", "FinalDestination", "West Standard");
        nodeMap["westStandard"] = root->rightChild->rightChild->rightChild;
    }
    
    // Route the crop through the decision tree
    DecisionNode* routeCrop(const Crop& crop, TransactionNode* transaction) {
        DecisionNode* current = root;
        string path = "root";
        
        // Calculate demand for this crop in its region
        float demand = getRegionalDemand(crop.areaCode, crop.type);
        string demandLevel = (demand >= 7.0) ? "High" : "Low";
        
        // Record demand info
        transaction->actionTaken += " Regional demand: " + to_string(demand) + "/10 (" + demandLevel + ")";
        
        // Traverse the tree until we reach a leaf node (no children)
        while (current != nullptr && (current->leftChild != nullptr || current->rightChild != nullptr)) {
            bool decision = current->decisionFunction(crop);
            
            // Record the decision in transaction
            transaction->actionTaken += " | " + current->nodeId + 
                                       " decision: " + (decision ? "left" : "right");
            
            // Move to next node based on decision
            if (decision) {
                path += " -> " + current->leftChild->nodeId;
                current = current->leftChild;
            } else {
                path += " -> " + current->rightChild->nodeId;
                current = current->rightChild;
            }
        }
        
        // Now current is a leaf node or null
        if (current != nullptr) {
            // Add this transaction to the queue of the final node
            current->enqueue(transaction);
            
            // Set next destination in the transaction
            transaction->nextDestination = "Node: " + current->nodeId + " (" + current->description + ")";
            
            // Add routing path to transaction action
            transaction->actionTaken += " | Final path: " + path;
        }
        
        return current;
    }
    
    // Get regional demand for a crop
    float getRegionalDemand(const string& region, const string& cropType) {
        // Check if we have demand data for this region and crop
        if (regionalDemand.find(region) != regionalDemand.end() &&
            regionalDemand[region].find(cropType) != regionalDemand[region].end()) {
            return regionalDemand[region][cropType];
        }
        
        // Default demand if not specified
        return 5.0; // Medium demand
    }
    
    // Get all nodes with their queue sizes
    vector<pair<string, int>> getAllQueueSizes() {
        vector<pair<string, int>> result;
        for (const auto& pair : nodeMap) {
            DecisionNode* node = pair.second;
            // Only include leaf nodes (processing nodes)
            if (node->leftChild == nullptr && node->rightChild == nullptr) {
                result.push_back({pair.first + " (" + node->description + ")", node->queueSize()});
            }
        }
        return result;
    }
    
    // Get list of leaf nodes with items in queue
    vector<pair<string, DecisionNode*>> getNodesWithItems() {
        vector<pair<string, DecisionNode*>> result;
        for (const auto& pair : nodeMap) {
            DecisionNode* node = pair.second;
            // Only include leaf nodes with items
            if (node->leftChild == nullptr && node->rightChild == nullptr && node->queueSize() > 0) {
                result.push_back({pair.first + " (" + node->description + ")", node});
            }
        }
        return result;
    }
    
    // Get a specific node by ID
    DecisionNode* getNode(const string& nodeId) {
        auto it = nodeMap.find(nodeId);
        if (it != nodeMap.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    // Display the tree structure
    void displayTreeStructure() {
        cout << "\n===== BINARY TREE STRUCTURE =====" << endl;
        displayNode(root, "", true);
    }
    
private:
    // Helper function to display a node and its children
    void displayNode(DecisionNode* node, string prefix, bool isRoot) {
        if (node == nullptr) return;
        
        cout << prefix;
        
        if (isRoot) {
            cout << "ROOT: ";
        } else {
            cout << "+--- ";
        }
        
        cout << node->nodeId << " (" << node->description << ")";
        if (node->queueSize() > 0) {
            cout << " [Queue: " << node->queueSize() << "]";
        }
        cout << endl;
        
        // Display children with appropriate indentation
        if (node->leftChild != nullptr || node->rightChild != nullptr) {
            string newPrefix = prefix + (isRoot ? "" : "|    ");
            
            if (node->leftChild != nullptr) {
                displayNode(node->leftChild, newPrefix, false);
            } else {
                cout << newPrefix << "+--- NULL" << endl;
            }
            
            if (node->rightChild != nullptr) {
                displayNode(node->rightChild, newPrefix, false);
            } else {
                cout << newPrefix << "+--- NULL" << endl;
            }
        }
    }
};

class AgriculturalSupplyChainApp {
private:
    TraceabilityChain traceabilityChain;
    RoutingDecisionTree routingTree;
    vector<string> areaCodes = {"North", "South", "East", "West"};
    
    // Generate unique IDs
    string generateUniqueId(string prefix) {
        static int counter = 1000;
        return prefix + to_string(++counter);
    }
    
public:
    // Farmer input flow
    void farmerInputCrop() {
        // In a real app, this would be from a form or API
        Crop newCrop;
        newCrop.id = generateUniqueId("CROP");
        
        cout << "Enter crop type: ";
        cin >> newCrop.type;
        
        cout << "Enter quantity (kg): ";
        cin >> newCrop.quantity;
        
        cout << "Enter freshness (1-10): ";
        float freshness;
        cin >> freshness;
        newCrop.qualityMetrics["freshness"] = freshness;
        
        cout << "Is organic? (1=yes, 0=no): ";
        int isOrganic;
        cin >> isOrganic;
        if (isOrganic) {
            newCrop.certifications.push_back("Organic");
        }
        
        cout << "Enter farmer ID: ";
        cin >> newCrop.farmerId;
        
        cout << "Enter location: ";
        cin >> newCrop.originLocation;
        
        // Select area code
        cout << "Select area code:" << endl;
        for (int i = 0; i < areaCodes.size(); i++) {
            cout << i+1 << ". " << areaCodes[i] << endl;
        }
        
        int areaIndex;
        cout << "Choice (1-" << areaCodes.size() << "): ";
        cin >> areaIndex;
        
        if (areaIndex >= 1 && areaIndex <= areaCodes.size()) {
            newCrop.areaCode = areaCodes[areaIndex-1];
        } else {
            newCrop.areaCode = "North"; // Default
        }
        
        newCrop.harvestDate = time(nullptr); // Current time
        
        processFarmerCrop(newCrop);
    }
    
    // Process crop from farmer
    void processFarmerCrop(const Crop& crop) {
        // Create first transaction in chain
        TransactionNode* farmerNode = new TransactionNode(
            generateUniqueId("TRANS"),
            crop.farmerId,
            "Farmer",
            crop.originLocation,
            "Initial harvest entry",
            crop
        );
        
        // Add to traceability chain
        traceabilityChain.addTransaction(farmerNode);
        
        // Route through decision tree and add to the appropriate node's queue
        DecisionNode* finalNode = routingTree.routeCrop(crop, farmerNode);
        
        cout << "\nCrop entered successfully!" << endl;
        cout << "Crop ID: " << crop.id << " (save this for tracking)" << endl;
        cout << "Destination node: " << finalNode->nodeId << " - " << finalNode->description << endl;
    }
    
    // Display all queues and their sizes
    void displayQueueStatus() {
        vector<pair<string, int>> queueSizes = routingTree.getAllQueueSizes();
        
        cout << "\n===== QUEUE STATUS =====" << endl;
        if (queueSizes.empty()) {
            cout << "No processing queues available." << endl;
            return;
        }
        
        for (const auto& pair : queueSizes) {
            cout << "Node: " << pair.first << " - Items in queue: " << pair.second << endl;
        }
    }
    
    // Display the binary tree structure
    void displayTreeStructure() {
        routingTree.displayTreeStructure();
    }
    
    // Trader processing flow
    void processTraderDecision() {
        // Display all nodes with items in queue
        vector<pair<string, DecisionNode*>> availableNodes = routingTree.getNodesWithItems();
        
        cout << "\n===== AVAILABLE QUEUES WITH CROPS =====" << endl;
        if (availableNodes.empty()) {
            cout << "No crops available in any queue." << endl;
            return;
        }
        
        for (int i = 0; i < availableNodes.size(); i++) {
            cout << i + 1 << ". " << availableNodes[i].first 
                 << " - Items: " << availableNodes[i].second->queueSize() << endl;
        }
        
        // Select a node to process
        cout << "Select node to process (1-" << availableNodes.size() << "): ";
        int nodeIndex;
        cin >> nodeIndex;
        
        if (nodeIndex < 1 || nodeIndex > availableNodes.size()) {
            cout << "Invalid node selection." << endl;
            return;
        }
        
        DecisionNode* selectedNode = availableNodes[nodeIndex-1].second;
        
        // Get next transaction from the selected node's queue
        TransactionNode* prevTransaction = selectedNode->dequeue();
        
        if (prevTransaction == nullptr) {
            cout << "No crops available in this queue." << endl;
            return;
        }
        
        // Display crop information
        cout << "\n===== CROP DETAILS =====" << endl;
        prevTransaction->cropDetails.display();
        
        // Trader information
        string traderId, location, decision;
        
        cout << "\nEnter trader ID: ";
        cin >> traderId;
        
        cout << "Enter current location: ";
        cin >> location;
        
        cout << "Select decision:" << endl;
        cout << "1. Route to Manufacturer" << endl;
        cout << "2. Route to Retailer" << endl;
        cout << "3. Route to Export" << endl;
        cout << "Choice: ";
        
        int decisionCode;
        cin >> decisionCode;
        
        if (decisionCode == 1) {
            decision = "Route to Manufacturer";
        } else if (decisionCode == 2) {
            decision = "Route to Retailer";
        } else {
            decision = "Route to Export";
        }
        
        // Create new transaction
        TransactionNode* traderNode = new TransactionNode(
            generateUniqueId("TRANS"),
            traderId,
            "Trader",
            location,
            decision,
            prevTransaction->cropDetails  // Same crop details
        );
        
        // Add to traceability chain, linking with previous transaction
        traceabilityChain.addTransaction(traderNode, prevTransaction);
        
        cout << "\nTrader decision processed successfully!" << endl;
        cout << "Transaction ID: " << traderNode->transactionId << endl;
    }
    
    // View crop history
    void viewCropHistory() {
        // First show all available crops
        traceabilityChain.listAllCrops();
        
        string cropId;
        cout << "\nEnter crop ID to trace: ";
        cin >> cropId;
        
        vector<TransactionNode*> history = traceabilityChain.getHistory(cropId);
        
        if (history.empty()) {
            cout << "No history found for this crop." << endl;
            return;
        }
        
        cout << "\n===== CROP HISTORY =====" << endl;
        for (int i = 0; i < history.size(); i++) {
            TransactionNode* node = history[i];
            cout << "Transaction " << i+1 << ": " << node->transactionId << endl;
            cout << "  Time: " << ctime(&node->timestamp);
            cout << "  Handler: " << node->handlerType << " (" << node->handlerId << ")" << endl;
            cout << "  Location: " << node->location << endl;
            cout << "  Action: " << node->actionTaken << endl;
            if (!node->nextDestination.empty()) {
                cout << "  Next Destination: " << node->nextDestination << endl;
            }
            cout << "------------------------" << endl;
        }
    }
    
    // Main menu
    void run() {
        while (true) {
            cout << "\n===== AGRICULTURAL SUPPLY CHAIN =====" << endl;
            cout << "1. Farmer: Enter New Crop" << endl;
            cout << "2. Trader: Process Crop" << endl;
            cout << "3. View Crop History" << endl;
            cout << "4. View Queue Status" << endl;
            cout << "5. View Binary Tree Structure" << endl;
            cout << "6. List All Crops" << endl;
            cout << "7. Exit" << endl;
            cout << "Choice: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1:
                    farmerInputCrop();
                    break;
                case 2:
                    processTraderDecision();
                    break;
                case 3:
                    viewCropHistory();
                    break;
                case 4:
                    displayQueueStatus();
                    break;
                case 5:
                    displayTreeStructure();
                    break;
                case 6:
                    traceabilityChain.listAllCrops();
                    break;
                case 7:
                    cout << "Exiting program." << endl;
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};

// Main function
int main() {
    AgriculturalSupplyChainApp app;
    app.run();
    return 0;
}