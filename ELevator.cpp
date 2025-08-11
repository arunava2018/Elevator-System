#include <bits/stdc++.h>
#include "Direction.cpp"
#include "ElevatorState.cpp"
#include "ExternalRequest.cpp"
using namespace std;
class Elevator{
public:
    int id;
    int currentFloor;
    ElevatorState state;
    int maxCapacity;
    int currentLoad;
    int totalFloors;
    set<int> upStops;                 // ascending
    set<int, greater<int>> downStops; // descending
    set<int> pickupFloor;

    Elevator(int id, int startFloor, int cap, int totalFloors)
        : id(id), currentFloor(startFloor), state(ElevatorState::IDLE),
          maxCapacity(cap), currentLoad(0), totalFloors(totalFloors) {}

    int getCurrentFloor() const { return currentFloor; }
    ElevatorState getState() const { return state; }
    int getId() const { return id; }
    bool hasSpace() const { return currentLoad < maxCapacity; }
    bool isIdle() const { return state == ElevatorState::IDLE; }

    // Calculate cost to handle an external request (intermediate stops are free)
    int calculateCost(const ExternalRequest &req) const
    {
        if (!hasSpace())
            return INT_MAX;

        if (state == ElevatorState::IDLE)
        {
            return abs(currentFloor - req.floor);
        }

        if (state == ElevatorState::MOVING_UP)
        {
            if (req.floor >= currentFloor)
            {
                return req.floor - currentFloor; // same direction
            }
            else
            {
                // will finish upStops first, then go down
                if (upStops.empty())
                    return abs(currentFloor - req.floor);
                return (*upStops.rbegin() - currentFloor) + abs(*upStops.rbegin() - req.floor);
            }
        }

        if (state == ElevatorState::MOVING_DOWN)
        {
            if (req.floor <= currentFloor)
            {
                return currentFloor - req.floor; // same direction
            }
            else
            {
                // will finish downStops first, then go up
                if (downStops.empty())
                    return abs(currentFloor - req.floor);
                return (currentFloor - *downStops.rbegin()) + abs(*downStops.rbegin() - req.floor);
            }
        }

        return INT_MAX;
    }

    void passengerEnter()
    {
        if (hasSpace())
        {
            int dest;
            cout << "Passenger entered Elevator " << id << ". Enter destination floor: ";
            // this cin will stop the code until the input is given, a passenger can enter and might not press the destination button
            // build a mechanism that can handle input at any time and insert it into upStop OR downStop
            cin >> dest;
            addInternalRequest(dest);
            pickupFloor.erase(currentFloor);
            currentLoad++;
        }
        else
        {
            cout << "Elevator " << id << " is full! Cannot take more passengers.\n";
        }
    }

    void addInternalRequest(int dest)
    {
        if (dest < 0 || dest >= totalFloors || dest == currentFloor)
            return;
        if (dest > currentFloor)
            upStops.insert(dest);
        else
            downStops.insert(dest);
    }

    void step()
    {
        if (!upStops.empty() || !downStops.empty()){
            if (state == ElevatorState::IDLE){
                if (!upStops.empty()){
                    state = ElevatorState::MOVING_UP;
                }
                else{
                    state = ElevatorState::MOVING_DOWN;
                }
            }

            if (state == ElevatorState::MOVING_UP){
                int target = *upStops.begin();
                currentFloor++;
                if (currentFloor == target){
                    stopAtFloorUp();
                    if (upStops.empty() && !downStops.empty())
                        state = ElevatorState::MOVING_DOWN;
                    else if (upStops.empty() && downStops.empty())
                        state = ElevatorState::IDLE;
                }
                // check if a passenger is needed to be onboarded at this floor
                if (currentFloor == *pickupFloor.begin())
                    onboardPassenger();
            }
            else if (state == ElevatorState::MOVING_DOWN){
                int target = *downStops.begin();
                currentFloor--;
                if (currentFloor == target){
                    stopAtFloorDown();
                    if (downStops.empty() && !upStops.empty()){
                        state = ElevatorState::MOVING_UP;
                    }
                    else if (downStops.empty() && upStops.empty()){
                        state = ElevatorState::IDLE;
                    }   
                }
                // check if a passenger is needed to be onboarded at this floor
                if (currentFloor == *pickupFloor.begin()){
                    onboardPassenger();
                }
                    
            }
        }
        else{
            state = ElevatorState::IDLE;
        }
    }

private:
    void stopAtFloorUp()
    {
        cout << "Elevator " << id << " dropped passenger at floor " << currentFloor << "\n";
        currentLoad = max(0, currentLoad - 1);
        upStops.erase(currentFloor);
    }

    void stopAtFloorDown()
    {
        cout << "Elevator " << id << " dropped passenger at floor " << currentFloor << "\n";
        currentLoad = max(0, currentLoad - 1);
        downStops.erase(currentFloor);
    }

    // function to load passenger
    void onboardPassenger()
    {
        cout << "Elevator " << id << " onboarding passenger at floor " << currentFloor << "\n";
        passengerEnter();
    }
};