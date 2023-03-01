#include "flipbip39_haptic.h"
#include "../flipbip39.h"


void flipbip39_play_happy_bump(void* context) {
    FlipBip39* app = context;
    if (app->haptic != 1) {
        return;
    }
    notification_message(app->notification, &sequence_set_vibro_on);
    furi_thread_flags_wait(0, FuriFlagWaitAny, 20);
    notification_message(app->notification, &sequence_reset_vibro);
}

void flipbip39_play_bad_bump(void* context) {
    FlipBip39* app = context;
    if (app->haptic != 1) {
        return;
    }
    notification_message(app->notification, &sequence_set_vibro_on);
    furi_thread_flags_wait(0, FuriFlagWaitAny, 100);
    notification_message(app->notification, &sequence_reset_vibro);
}

void flipbip39_play_long_bump(void* context) {
    FlipBip39* app = context;
    if (app->haptic != 1) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        notification_message(app->notification, &sequence_set_vibro_on);
        furi_thread_flags_wait(0, FuriFlagWaitAny, 50);
        notification_message(app->notification, &sequence_reset_vibro);
        furi_thread_flags_wait(0, FuriFlagWaitAny, 100);
    }
}