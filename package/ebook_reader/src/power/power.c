#include <stdlib.h>

#include "app/app.h"
#include "core.h"
#include "display/display.h"
#include "event_queue/event_queue.h"
#include "power/power.h"
#include "utils/err.h"
#include "utils/log.h"

enum PowerStates {
  PowerStates_NONE,
  PowerStates_ACTIVE,
  PowerStates_MAX,
};

struct Power {
  enum PowerStates current_state;
  event_queue_t evqueue;
  display_t display;
  struct PowerView view;
};

struct PowerTransition {
  enum PowerStates next_state;
  post_event_func_t action;
};

static void power_post_event(enum Events event, ref_t event_data,
                             void *sub_data);
static const char *power_state_dump(enum PowerStates state);
static void power_off(enum Events __, ref_t ___, void *sub_data);
static void power_handle_power_button(enum Events __, ref_t ___,
                                      void *sub_data);

struct PowerTransition power_fsm_table[PowerStates_MAX][Events_MAX] = {
    [PowerStates_NONE] =
        {
            [Events_BTN_POWER_CLICKED] =
                {
                    .next_state = PowerStates_ACTIVE,
                    .action = power_handle_power_button,
                },
        },
    [PowerStates_ACTIVE] =
        {
            [Events_POWER_OFF] =
                {
                    .next_state = PowerStates_ACTIVE,
                    .action = power_off,
                },

        },
};

err_t power_init(power_t *out, display_t display, event_queue_t queue) {
  log_info("%s", __func__);
  power_t power = *out = mem_malloc(sizeof(struct Power));
  *power = (struct Power){
      .current_state = PowerStates_NONE,
      .evqueue = queue,
      .display = display,
  };

  event_queue_register(queue, EventSubscribers_POWER, power_post_event, power);
  return 0;
}

void power_destroy(power_t *out) {
  if (mem_is_null_ptr(out)) {
    return;
  }

  power_t power = *out;
  event_queue_deregister(power->evqueue, EventSubscribers_POWER);
  mem_free(*out);
  *out = NULL;
}

static void power_post_event(enum Events event, ref_t event_data,
                             void *sub_data) {

  struct PowerTransition action;
  power_t power = sub_data;

  action = power_fsm_table[power->current_state][event];
  if (!action.action) {
    return;
  }

  if (power->current_state != action.next_state) {
    log_info("%s -> %s", power_state_dump(power->current_state),
             power_state_dump(action.next_state));
  }

  action.action(event, event_data, sub_data);
  power->current_state = action.next_state;
}

static const char *power_state_dump(enum PowerStates state) {
  static char *dumps[PowerStates_MAX] = {
      [PowerStates_NONE] = "power_none",
      [PowerStates_ACTIVE] = "power_active",
  };

  if (state < PowerStates_NONE || state >= PowerStates_MAX || !dumps[state]) {
    return "Unknown";
  }

  return dumps[state];
};

static void power_off(enum Events __, ref_t ___, void *sub_data) {
  log_info("%s", __func__);
  /* power_t power = sub_data; */

  log_warn("POWER OFF!!!!");

  /* power_off_view_destroy(&power->view); */

  /* app_panic()  ; */
  /* if (system("poweroff") != 0) { */
    /* log_warn("POWER OFF FAILED!!!!"); */
  /* } */

  return;
}

static void power_handle_power_button(enum Events __, ref_t ___,
                                      void *sub_data) {
  power_t power = sub_data;
  err_o = power_off_view_init(&power->view);
  ERR_TRY(err_o);

  /* event_queue_push(power->evqueue, Events_POWER_OFF, power); */

  return;

error_out:
  log_error(err_o);
}
