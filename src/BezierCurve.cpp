#include "BezierCurve.h"
#include <iostream>

#include "helper.h"

#include <glm/gtc/matrix_transform.hpp>

BezierCurve::BezierCurve() {
    control_points.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
    control_points.push_back(glm::vec4(0.0f, 0.0f, +1.0f, 1.0f));
}

BezierCurve::~BezierCurve() {

}

void BezierCurve::setControlPoints(const std::vector<glm::vec3>& _control_points) {
  control_points.resize(_control_points.size());
  for (size_t i = 0; i < control_points.size(); i++)
    control_points[i] = glm::vec4(_control_points[i], 1.0f);
}

std::vector<glm::vec4>* BezierCurve::getSeedingPoints(const size_t& nSeeds) const {

  std::vector<glm::vec4>* points = new std::vector<glm::vec4>();

  for (size_t i = 0; i < nSeeds; i++){
    float t = static_cast<float>(i) / static_cast<float>(nSeeds - 1);
    points->push_back(m_model_to_world_transform * point_on_curve(t));
  }

    return points;
}

void BezierCurve::setPosition(const glm::vec3& newPos) {
    m_model_to_world_transform = glm::translate(glm::mat4(1.0f), newPos);
    m_world_to_model_transform = glm::inverse(m_model_to_world_transform);
}

glm::vec4 BezierCurve::point_on_curve(const float& t) const {
    std::vector<glm::vec4> tmp = control_points;
    int i = static_cast< int >( tmp.size() - 1 );
    while (i > 0) {
        for (int k = 0; k < i; k++)
            tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
        i--;
    }

    return glm::vec4(tmp[0].x, tmp[0].y, tmp[0].z, 1.0f);
}