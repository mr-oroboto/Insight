#ifndef INSIGHT_SCENARIO_SCENARIO_H
#define INSIGHT_SCENARIO_SCENARIO_H

namespace insight {

class DisplayManager;

namespace scenario {


class Scenario {
public:
    Scenario(DisplayManager* display_manager) : display_manager_(display_manager) {}
    virtual ~Scenario() = default;

    virtual void run() = 0;

protected:
    DisplayManager* display_manager_;
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_SCENARIO_H
