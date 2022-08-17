#include <iostream>
#include <fstream>

#define DEFAULT_SIZE 5;
#define DEFAULT_LOG_PATH "_common_log.txt"

using namespace std;

namespace servercluster {

    // Struct and Class Definitions
    #pragma region 

    typedef struct Request {
        int id;
        string message;
        void init(int, string);
        //void clear();
    } Request;

    typedef enum NeighbourType {
        NEXT,
        PREVIOUS
    } NeighbourType;

    class Node {
        private:
            int id; string name;
            int lowerLimit, upperLimit; // Process range of the node
            fstream logFile;
            Node* nextNode; Node* previousNode; // Neighbour nodes

            void sendToNeighbour(Request*); // Send request to a neighbour
            void processRequest(Request*); // Basically echo the message
            void logProcess(Request*);

        public:
            // Constructors and Destructor
            Node();
            Node(int, string, int, int, string);
            Node(int, string, int, int, string, Node*, Node*);
            ~Node();

            // Getters & Setters
            int getID();
            string getName();
            Node* getNeighbour(NeighbourType);
            void setNeighbour(Node*, NeighbourType);

            // Basic Methods
            void receiveRequest(Request*); // Receives a request from server or neighbour
    };

    class ServerCluster {
        private:
            int size;
            Node* headNode;
            Node* tailNode;
            int hashFunction(); // Request assigned to a server
            void changeReceiver(); // Change the next receiving server
        
        public:
            // Constructors and Destructor
            ServerCluster();
            ServerCluster(int);
            ~ServerCluster();
            
            // Basic Methods
            void addServer(Node*);
            void removeServer(string);
            void handleRequest(Request*);

            // Simulation
            void simulate();
    };

    #pragma endregion

    /* METHOD IMPLEMENTATIONS */
    #pragma region

    void Request::init(int requestID, string requestMessage) {
        id = requestID;
        message = requestMessage;
    }

    Node::Node() {
        this->id = -1;
        this->name = "";
        this->lowerLimit = INT16_MIN;
        this->upperLimit = INT16_MAX;
        this->logFile.open(DEFAULT_LOG_PATH, fstream::out);
        this->nextNode = nullptr;
        this->previousNode = nullptr;
    }

    Node::Node(int id, string name, int lowerLimit, int upperLimit, string logFilePath) {
        this->id = id;
        this->name = name;
        this->lowerLimit = lowerLimit;
        this->upperLimit = upperLimit;
        this->logFile.open(logFilePath, fstream::out);
        this->nextNode = nullptr;
        this->previousNode = nullptr;
    }

    Node::Node(int id, string name, int lowerLimit, int upperLimit, string logFilePath, Node* nextNode, Node* previousNode) {
        this->id = id;
        this->name = name;
        this->lowerLimit = lowerLimit;
        this->upperLimit = upperLimit;
        this->logFile.open(logFilePath, fstream::in | fstream::out);
        this->nextNode = nextNode;
        this->previousNode = previousNode;
    }

    Node::~Node() {
        this->logFile.close();
        this->nextNode = nullptr;
        this->previousNode = nullptr;
    }

    int Node::getID() {
        return this->id;
    }

    string Node::getName() {
        return this->name;
    }

    Node* Node::getNeighbour(NeighbourType neighbourType) {
        if (neighbourType == NEXT) {
            return this->nextNode;
        } else {
            return this->previousNode;
        }
    }

    void Node::setNeighbour(Node* node, NeighbourType neighbourType) {
        if (neighbourType == NEXT) {
            this->nextNode = node;
        } else {
            this->previousNode = node;
        }
    }

    void Node::receiveRequest(Request* request) {
        if (request->id < lowerLimit || request->id >= upperLimit) {
            this->sendToNeighbour(request);
        } else {
            this->processRequest(request);
            this->logProcess(request);
        }
    }

    void Node::sendToNeighbour(Request* request) {
        if (request->id < lowerLimit) {
            cout << "[REDIRECTED]\t-> Node#" << this->previousNode->id << endl;
            this->previousNode->receiveRequest(request);
        } else {
            cout << "[REDIRECTED]\t-> Node#" << this->nextNode->id << endl;
            this->nextNode->receiveRequest(request);
        }     
    }

    void Node::processRequest(Request* request) {
        cout << "[PROCESSED]";
        cout << "\tRequest Message: " << request->message << endl;
    }

    void Node::logProcess(Request* request) {
        this->logFile << "[NODE#" << this->name << "] ";
        this->logFile << "Request Processed - \"" << request->message << "\"" << endl;
    }

    ServerCluster::ServerCluster() {
        this->size = DEFAULT_SIZE;
        for (int n=0; n<this->size; n++) {
            if (n==0) {
                // HeadNode is TailNode
                this->headNode = new Node(n, "InitialNode", n, n+1, DEFAULT_LOG_PATH);
                this->tailNode = this->headNode;
            } else {
                // Add next to TailNode and become TailNode
                Node* newNode = new Node(n, "Node", n, n+1, DEFAULT_LOG_PATH);
                this->tailNode->setNeighbour(newNode, NEXT);
                newNode->setNeighbour(this->tailNode, PREVIOUS);
                this->tailNode = newNode;
            }
            
            // NextNode of TailNode is HeadNode
            // PreviousNode of HeadNode is TailNode
            this->tailNode->setNeighbour(this->headNode, NEXT);
            this->headNode->setNeighbour(this->tailNode, PREVIOUS);
        }
    }

    ServerCluster::ServerCluster(int size) {
        this->size = size;
        for (int n=0; n<this->size; n++) {
            if (n==0) {
                // HeadNode is TailNode
                this->headNode = new Node(n, "InitialNode", n, n+1, DEFAULT_LOG_PATH);
                this->tailNode = this->headNode;
            } else {
                // Add next to TailNode and become TailNode
                Node* newNode = new Node(n, "Node", n, n+1, DEFAULT_LOG_PATH);
                this->tailNode->setNeighbour(newNode, NEXT);
                newNode->setNeighbour(this->tailNode, PREVIOUS);
                this->tailNode = newNode;
            }
            
            // NextNode of TailNode is HeadNode
            // PreviousNode of HeadNode is TailNode
            this->tailNode->setNeighbour(this->headNode, NEXT);
            this->headNode->setNeighbour(this->tailNode, PREVIOUS);
        }
    }

    ServerCluster::~ServerCluster() {
        Node* nextNode;
        Node* traverse = this->headNode;
        while (traverse != nullptr) {
            nextNode = traverse->getNeighbour(NEXT);
            delete traverse;
            traverse = nextNode;
        }
    }

    void ServerCluster::addServer(Node* newNode) {
        // Add next to TailNode and become TailNode
        this->tailNode->setNeighbour(newNode, NEXT);
        newNode->setNeighbour(this->tailNode, PREVIOUS);
        this->tailNode = newNode;

        // NextNode of TailNode is HeadNode
        // PreviousNode of HeadNode is TailNode
        this->tailNode->setNeighbour(this->headNode, NEXT);
        this->headNode->setNeighbour(this->tailNode, PREVIOUS);
        this->size++;
    }

    void ServerCluster::removeServer(string nodeName) {
        Node* traverse = this->headNode;
        while (traverse != this->tailNode) {
            if (traverse->getName() == nodeName) {
                traverse->getNeighbour(PREVIOUS)->setNeighbour(
                    traverse->getNeighbour(NEXT), NEXT);
                traverse->getNeighbour(NEXT)->setNeighbour(
                    traverse->getNeighbour(PREVIOUS), PREVIOUS);
                delete traverse;
                break;
            }
        }

        // Query for TailNode
        if (traverse->getName() == nodeName) {
            traverse->getNeighbour(PREVIOUS)->setNeighbour(
                traverse->getNeighbour(NEXT), NEXT);
            traverse->getNeighbour(NEXT)->setNeighbour(
                traverse->getNeighbour(PREVIOUS), PREVIOUS);
            delete traverse;
        }
    }

    void ServerCluster::handleRequest(Request* request) {
        cout << "[REQUESTED]";
        cout << "\t->" << "Node#" << this->headNode->getID() << endl;
        this->headNode->receiveRequest(request);
    }

    void ServerCluster::simulate() {
        Request* newRequest = new Request;
        newRequest->init(3, "Hello World!");
        this->handleRequest(newRequest);
    }

    #pragma endregion
};