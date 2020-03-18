#include "ETransform.hpp"
#include "Logging.hpp"

// -- constructors -------------------------------------------------------------
ETransform::ETransform(glm::vec3 pos, glm::vec3 scale, glm::quat rotQ)
: _pos(pos),
  _rot(rotQ),
  _scale(scale)
{
	_updateModel();
}

ETransform::ETransform(glm::vec3 pos, glm::vec3 scale, glm::mat4 rotM)
: ETransform(pos, scale, glm::toQuat(rotM)) {}

ETransform::ETransform(glm::vec3 pos, glm::vec3 scale, float rotAngle,
glm::vec3 rotAxis)
: ETransform(pos, scale, glm::rotate(glm::mat4(1.0), rotAngle, rotAxis)) {}

ETransform::~ETransform() {
}

ETransform::ETransform(ETransform const &src) {
	*this = src;
}

ETransform &ETransform::operator=(ETransform const &rhs) {
	if (this != &rhs) {
		_pos = rhs._pos;
		_rot = rhs._rot;
		_scale = rhs._scale;
		_model = rhs._model;
	}

	return *this;
}

// -- _updateModel -------------------------------------------------------------
void	ETransform::_updateModel() {
	// translation
	_model = glm::translate(glm::mat4(1.0f), _pos);
	// rotation
	_model = _model * glm::toMat4(_rot);
	// scale
	_model = glm::scale(_model, _scale);
}

// -- setters ------------------------------------------------------------------
/**
 * @brief change the transform position and update the model matrix
 *
 * @param pos the new position
 */
void	ETransform::setPos(glm::vec3 const pos) {
	_pos = pos;
	_updateModel();
}

/**
 * @brief change the transform rotation and update the model matrix
 *
 * @param rot the new rotation quaternion
 */
void	ETransform::setRot(glm::quat const rot) {
	_rot = rot;
	_updateModel();
}

/**
 * @brief change the transform rotation and update the model matrix
 *
 * @param rotM the new rotation matrix
 */
void	ETransform::setRot(glm::mat4 const rotM) {
	_rot = glm::toQuat(rotM);
	_updateModel();
}

/**
 * @brief change the transform rotation and update the model matrix
 *
 * @param rotAngle the new rotation angle
 * @param rotAxis the new rotation axis
 */
void	ETransform::setRot(float const rotAngle, glm::vec3 const rotAxis) {
	_rot = glm::toQuat(
		glm::rotate(glm::mat4(1.0), rotAngle, rotAxis));
		_updateModel();
}

/**
 * @brief change the transform scale and update the model matrix
 *
 * @param scale the new scale value
 */
void	ETransform::setScale(glm::vec3 const scale) {
	_scale = scale;
	_updateModel();
}

// -- getters ------------------------------------------------------------------
glm::vec3 const	ETransform::getPos() const { return _pos; }
glm::quat const	ETransform::getRot() const { return _rot; }
glm::vec3 const	ETransform::getScale() const { return _scale; }
glm::mat4 const	ETransform::getModel() const { return _model; }
