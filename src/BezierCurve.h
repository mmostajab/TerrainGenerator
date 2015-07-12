#ifndef __SEEDING_CURVE_H__
#define __SEEDING_CURVE_H__

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class BezierCurve {
public:
    BezierCurve();
    ~BezierCurve();

    void setControlPoints(const std::vector<glm::vec3>& _control_points);
    void setPosition(const glm::vec3& newPos);

    std::vector<glm::vec4>* getSeedingPoints(const size_t& nSeeds) const;
    

    glm::vec4 point_on_curve(const float& t) const;

protected:
    std::vector<glm::vec4> control_points;

    glm::mat4 m_world_to_model_transform;
    glm::mat4 m_model_to_world_transform;
};

#endif
