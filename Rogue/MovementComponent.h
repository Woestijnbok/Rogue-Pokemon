#pragma once

// Libraries
#include  <optional>
#include <vec2.hpp>

// Core
#include "Component.h"
#include "Subject.h"

// Other
#include "Enums.h"

class TileManagerComponent;

/*
* Implements up, right, down and left tile based movement.
*/
class MovementComponent final : public Minigin::Component
{
public:
	/*
	* Constructs movement component.
	* 
	* @param owner: The owner of the movement component.
	* @param tileManager: The tile manager.
	*/
	explicit MovementComponent(Minigin::GameObject* owner, TileManagerComponent* tileManager);
	virtual ~MovementComponent() = default;

	MovementComponent(const MovementComponent& other) = delete;
	MovementComponent(MovementComponent&& other) noexcept = delete;
	MovementComponent& operator=(const MovementComponent& other) = delete;
	MovementComponent& operator=(MovementComponent&& other) noexcept = delete;

	/*
	* Moves the owning game object if we are in a moving status.
	*/
	virtual void Update() override;

	/*
	* Sets the component in the move state and updates sprite component accordingly.
	* This only happens if we are not already moving and can move in the given direction.
	* 
	* @param direction: The direction to move in.
	*/
	void Move(Direction direction);
	/*
	* Gets the current direction, could be the latest use IsMoving() to check.
	* 
	* @return The current direction.
	*/
	Direction GetDirection() const;
	/*
	* Checks if we are moving or not.
	* 
	* @return Wether or not we are moving, true = moving.
	*/
	bool IsMoving() const;
	/*
	* Gets the speed of the movement component.
	*
	* @return The speed to use for movement.
	*/
	float GetSpeed() const;
	/*
	* Sets the speed for the movement component.
	* 
	* @param speed: The speed to use for movement.
	*/
	void SetSpeed(float speed);
	/*
	* Gets the on move started event.
	* 
	* @return Move started event.
	*/
	Minigin::Subject<Direction>& OnMoveStarted();
	/*
	* Gets the on move completed event.
	*
	* @return Move completed event.
	*/
	Minigin::Subject<>& OnMoveCompleted();

private:
	/*
	* Sets the current direction.
	* 
	* @param direction: The direction to set.
	*/
	void SetDirection(Direction direction);
	/*
	* Teleports the owning game object to the start tile.
	*/
	void TeleportToStartTile();
	/*
	* Finishes movement, clears target position and stops sprite update.
	* Mainly here as a helper to have snappy movement and never go to far.
	* 
	* @param newPosition: The position that will be set to the finished target position
	*/
	void CompleteMovement(glm::ivec2& newPosition);

	// Current direction
	Direction m_Direction;
	// If we are moving
	bool m_Moving;
	// The speed we move in
	float m_Speed;
	// Current target position
	std::optional<glm::ivec2> m_TargetPosition;
	// Cached tile manager component
	TileManagerComponent* m_TileManagerComponent;
	// Move started event
	Minigin::Subject<Direction> m_OnMoveStarted;
	// Move completed event
	Minigin::Subject<> m_OnMoveCompleted;
};