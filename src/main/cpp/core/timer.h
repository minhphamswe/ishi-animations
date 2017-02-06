/** @file timer.h
 *
 * Declares the Timer class for high-resolution timer
 */
#ifndef ISHI_CORE_CLOCK_H_
#define ISHI_CORE_CLOCK_H_

namespace ishi {

class Timer {
 private:
  int64 lastTime;
  float delta;

 public:
  explicit Timer(float startTimeSeconds = 0.0f);

  /// Return number of ticks per second
  int64 secondsToCycles(float timeSeconds);
  float cyclesToSeconds(int64 timeCycles);

  int64 getTimeCycles() const;

  /// Return the second difference between this and another timer
  float SecDiff(const Timer& other) const;
};

/// Initialize the timer system, get hardware timer frequency, etc.
/// @todo Try to skip this step
int InitTimer();

/// Return time elapsed per tick
int32 Resolution();

/// Advance the timer
float Tick(const Timer& t);

/// Take the current time
int64 Time();


}  // namespace ishi

#endif
