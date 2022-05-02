#include "autonomous/odometry.hpp"
#include "subsystems/chassis.hpp"
#include "subsystems/highLift.hpp"
#include "subsystems/powershare.hpp"
#include "autonomous/autonomous.hpp"

#define kPneumaticClampPort 7
#define kPneumaticTilterPort 1
#define kPneumaticTilterPort2 6
#define kPneumaticTransmissionPort 3
#define kPneumaticCoverPort 4

/*
pros::c::ext_adi_digital_write(2, kPneumaticClampPort, LOW);
pros::c::adi_digital_write(kPneumaticTilterPort, LOW);
pros::c::ext_adi_digital_write(2, kPneumaticTilterPort2, LOW);
pros::c::adi_digital_write(kPneumaticTransmissionPort, LOW);
pros::c::adi_digital_write(kPneumaticCoverPort, LOW);
*/

// functions
void highLiftTask(void* ignore) {
  continueHighLift = true;
  highTogglePosition();
}
void lowLiftTask(void* ignore) {
  continueLowLift = true;
  lowTogglePosition();
}
void giveUp() {
  continueHighLift = true;
  while (continueHighLift) {
    if (chassis->getState().x.convert(okapi::foot) >= 5.5) {
      pros::c::ext_adi_digital_write(2, kPneumaticClampPort, HIGH);
    }
  }
}
void tilt() {
  pros::c::adi_digital_write(kPneumaticTilterPort, HIGH);
  pros::delay(500);
  pros::c::ext_adi_digital_write(2, kPneumaticTilterPort2, HIGH);
}
void untilt() {
  pros::c::ext_adi_digital_write(2, kPneumaticTilterPort2, LOW);
  pros::delay(500);
  pros::c::adi_digital_write(kPneumaticTilterPort, LOW);
}
void place() {
}

void right() {
  chassis->setState({0_ft, 0_ft});
  pros::c::adi_digital_write(kPneumaticTransmissionPort, LOW);
  pros::c::adi_digital_write(kPneumaticTilterPort, LOW);
  pros::c::ext_adi_digital_write(2, kPneumaticClampPort, LOW);
  pros::c::ext_adi_digital_write(2, kPneumaticTilterPort2, LOW);
  pros::c::adi_digital_write(kPneumaticCoverPort, HIGH);
  pros::Task highLift(highLiftTask);
  state = 0;
  pros::Task giveUpTask(giveUp);
  jCurve(3.3, 0, true, 0, 1, 2, false, true);
  pros::c::ext_adi_digital_write(2, kPneumaticClampPort, HIGH);
  pros::delay(100);
  // state = 3;
  jCurve(0.8, 0, false, 0, 1, 1.5, true);
  state = 0;
  // pros::delay(200);
  pros::c::ext_adi_digital_write(2, kPneumaticClampPort, LOW);
  pros::c::adi_digital_write(kPneumaticCoverPort, LOW);
  jCurve(-0.6, 0, false, 0, 1, 0.7);
  pros::c::adi_digital_write(kPneumaticCoverPort, HIGH);
  jCurve(3, -2.3, true, 0, 1, 2.5, false, true);
  pros::c::ext_adi_digital_write(2, kPneumaticClampPort, HIGH);
  pros::delay(100);
  state = 3;
  jCurve(0, -0.008, false, 0, 1, 2, true);
  state = 0;
  pros::delay(200);
  pros::c::ext_adi_digital_write(2, kPneumaticClampPort, LOW);
  pros::c::adi_digital_write(kPneumaticCoverPort, LOW);
  jCurve(1.65, 1.75, false, 0, 1, 1.5);
  pros::c::adi_digital_write(kPneumaticTilterPort, HIGH);
  pros::delay(200);
  pros::c::ext_adi_digital_write(2, kPneumaticTilterPort2, HIGH);
  state = 3;
  powershare.controllerSet(-1);
  relative(2, 0.3);
}
void rightOne() {
  okapi::Timer timer;
  chassis->setState({0_ft, 0_ft});
  pros::c::adi_digital_write(kPneumaticTransmissionPort, LOW);
  pros::c::adi_digital_write(kPneumaticTilterPort, LOW);
  pros::c::ext_adi_digital_write(2, kPneumaticClampPort, LOW);
  pros::c::ext_adi_digital_write(2, kPneumaticTilterPort2, LOW);
  pros::c::adi_digital_write(kPneumaticCoverPort, HIGH);
  pros::Task highLift(highLiftTask);
  state = 0;
  pros::Task giveUpTask(giveUp);
  jCurve(3.3, 0, true, 0, 1, 2, false, true);
  pros::c::ext_adi_digital_write(2, kPneumaticClampPort, HIGH);
  pros::delay(100);
  state = 3;
  jCurve(1, 0, false, 0, 1, 2, true);
  imuTurnToAngle(-90);
  relative(-1.8, 1);
  pros::c::adi_digital_write(kPneumaticTilterPort, HIGH);
  pros::delay(100);
  state = 2;
  jCurve(1, 0.7, true, 0, 1, 0.7);
  imuTurnToAngle(0);
  pros::c::ext_adi_digital_write(2, kPneumaticTilterPort2, HIGH);
  if (timer.millis().convert(okapi::second) < 10) {
    powershare.controllerSet(-1);
    jCurve(4.2, 0.7, true, 0, 0.5, 3);
    powershare.controllerSet(0);
    jCurve(0.2, 0.7, false, 0, 1, 2, true); // drive back
    state = 0;
  }
}
void rightAllianceWP() {
}
void left() {
}
void leftOne() {
}
void leftCounter() {
}
void soloAWP() {
}
void skills() {
}
