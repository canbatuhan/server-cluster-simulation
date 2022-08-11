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

            // Basic Methods
            void setAsNeighbour(Node*, NeighbourType);
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

    // Function Implementations
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
        this->logFile.open(logFilePath, fstream::out);
        this->nextNode = nextNode;
        this->previousNode = previousNode;
    }

    Node::~Node() {
        this->logFile.close();
        this->nextNode = nullptr;
        this->previousNode = nullptr;
    }

    void Node::setAsNeighbour(Node* node, NeighbourType neighbourType) {
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
            this->processRequest(request);
        }
    }

    void Node::sendToNeighbour(Request* request) {
        if (request->id < lowerLimit) {
            this->previousNode->receiveRequest(request);
        } else {
            this->previousNode->receiveRequest(request);
        }     
    }

    void Node::processRequest(Request* request) {
        cout << "Request Type: " << request->id;
        cout << "\tRequest Message: " << request->message << endl;
    }

    void Node::logProcess(Request* request) {
        this->logFile << "[NODE#" << this->name << "] ";
        this->logFile << "Request Processed - \"" << request->message << "\"" << endl;
    }

    #pragma endregion
};