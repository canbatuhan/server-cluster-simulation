#include "include/ServerCluster.h"

using namespace servercluster;

int main() {
    servercluster::ServerCluster cluster = ServerCluster(10);
    cluster.simulate();
}