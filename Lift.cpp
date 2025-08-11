#include <bits/stdc++.h>
#include "Direction.cpp"
#include "ElevatorState.cpp"
#include "ExternalRequest.cpp"
#include "Elevator.cpp"
using namespace std;
class ElevatorController{
private:
    vector<Elevator *> elevators;
    int totalFloors;
public:
    ElevatorController(int numElevators, int capacity, int totalFloors)
        : totalFloors(totalFloors)
    {
        for (int i = 0; i < numElevators; i++)
        {
            elevators.emplace_back(i + 1, 0, capacity, totalFloors);
        }
    }

    void handleExternalRequest(const ExternalRequest &req)
    {
        Elevator best = findBestElevator(req);
        best.pickupFloor.insert(req.floor);
    }

    void stepSystem()
    {
        for (auto elevator : elevators)
        {
            elevator->step();
        }
    }

private:
    Elevator &findBestElevator(const ExternalRequest &req)
    {
        int minCost = INT_MAX;
        Elevator *chosen = nullptr;
        for (auto &e : elevators)
        {
            int cost = e->calculateCost(req);
            if (cost < minCost)
            {
                minCost = cost;
                chosen = e;
            }
        }
        return *chosen;
    }
};

int main()
{
    int totalFloors = 10;
    ElevatorController controller(2, 4, totalFloors);

    // should handle external request at any time of the lift state as in should take input at any moment.
    controller.handleExternalRequest(ExternalRequest(3, Direction::UP));
    controller.handleExternalRequest(ExternalRequest(7, Direction::DOWN));

    for (int i = 0; i < 20; i++)
    {
        cout << "--- Time step " << i << " ---\n";
        controller.stepSystem();
    }

    return 0;
}