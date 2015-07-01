#pragma once

class GameLoop;

class IGameObject{
 public:
	//! Called each frame.
	/*!
	 * Lets the GameObject add/change/remove its own ISceneNodes into
	 * the scene manager.
	 *
	 * Here you should interpolate all the values between the current
	 * state and the next state. To keep things simple you can just
	 * set the updated values into the ISceneNode at first.
	 *
	 * @param mgr: A reference to the global scene manager.
	 *
	 * @param tick_progress: The time that passed from the last logic
	 * tick, expressed as a range [0.0, 1.0).
	 */
	virtual void rendering_loop(GameLoop& mgr, float tick_progress) = 0;


	//! Called with a fixed rate of 100Hz
	/*!
	 * Here you should handle the logic, the network and all the
	 * non-graphic-related tasks.
	 */
	virtual void logic_tick(GameLoop& mgr) = 0;
};


template<typename State, typename Renderer>
class InterpolatingSplitGameObject : public IGameObject{

	// Initialized with the default constructor. If that's not good, I
	// feel sorry for you: call a method that really initializes them
	// after they have been instantiated by me.

	// Why use templates for this instead of using inheritance/virtual calls?
	// - 1: They are put in memory all in a single block, so it's more
	//      probable that the copying involved between them is faster
	//      due to smaller cache footprint
	// - 2: virtual function calls are SLOW!
	State prev_state, curr_state, next_state;
	Renderer renderer;


	explicit InterpolatingSplitGameObject() {
	}


	void rendering_loop(GameLoop& mgr, float tick_progress) {
		curr_state.interpolate(prev_state, next_state, tick_progress);
		renderer.render(mgr, curr_state);
	}


	void logic_tick(GameLoop& mgr) {
		prev_state.copy(next_state);
		curr_state.copy(prev_state);
		next_state.tick();
	}
};


template<typename State>
class IGameObjectState {
	//! Copies the state passed as parameter into this object
	virtual void copy(const State& game_state) = 0;

	//! Sets the internal state of this object to an interpolation
	//! between the two specified object
	/*!
	 * By default it simply copies the next tick state into the current one
	 */
	virtual void interpolate(const State& previous_tick, const State& next_tick, float progress) {
	    this->copy(next_tick);
    }

	//! Advances the state of this game object to the next "logical step"
	virtual void tick(GameLoop& mgr) = 0;
};


template<typename State>
class IGameObjectRenderer {
	//! Renders a state using irrlich.
	virtual void render(GameLoop& mgr, State& state) = 0;
};


// while( true )
// {
//     int elapsed_time = getcurrenttime() - oldtime;
//     if(elapsed_time >= minimum_elapsed_time)
//       break;
//     sleep(1); //yield cpu to other game thread or processes...
// }

