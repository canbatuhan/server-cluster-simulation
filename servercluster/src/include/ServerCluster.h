#include <iostream>
#include <fstream>

#define DEFAULT_SIZE 5;

namespace servercluster {

    // Struct and Class Definitions
    #pragma region 

    typedef struct Request {
        int id;
        std::string message;
        void init(int, std::string);
        //void clear();
    } Request;

    typedef enum NeighbourType {
        RIGHT,
        LEFT
    } NeighbourType;

    class Node {
        private:
            int id; std::string name;
            int lowerLimit, upperLimit; // Process range of the node
            std::fstream logFile;
            Node* nextNode; Node* previousNode; // Neighbour nodes
            void sendToNeighbour(Request*); // Send request to a neighbour
            void processRequest(Request*); // Basically echo the message
            void logProcess(Request*);

        public:
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
            void removeServer(std::string);
            void handleRequest(Request*);

            // Simulation
            void simulate();
    };

    #pragma endregion

    // Function Implementations
    #pragma region
    #pragma endregion
};