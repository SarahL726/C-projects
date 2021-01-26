#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendar.h"

/* Initialize a Calendar structure based on the parameters provided */
int init_calendar(const char *name, int days, int (*comp_func) (const void *ptr1, const void *ptr2), void (*free_info_func) (void *ptr), Calendar ** calendar){
    if (!calendar || !name || days < 1){
        return FAILURE;
    }
    
    /* Allocate memory for Calendar structure */
    (*calendar) = malloc(sizeof(Calendar));
    if (!calendar){
        return FAILURE;
    }
    
    /* Allocate memory for name and events */
    (*calendar)->name = (char *)malloc(strlen(name) + 1);
    (*calendar)->events = calloc(days, sizeof(Event));
    if (!(*calendar)->name || !(*calendar)->events){
        return FAILURE;
    }
    
    strcpy((*calendar)->name, name);
    (*calendar)->days = days;
    (*calendar)->total_events = 0;
    (*calendar)->comp_func = comp_func;
    (*calendar)->free_info_func = free_info_func;
    
    return SUCCESS;
}

/* Prints, to the disgnated output stream, the calendar's name, days, and total number of events if print_all is true; otherwise this information will not be printed. Information about each event (name, start time, and duration) will be printed regardless the value of print_all. Notice that the heading "**** Events ****" will always be printed. */
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all){
    Event *event;
    int i;
    
    if (!calendar || !output_stream){
        return FAILURE;
    }
    
    if (print_all){
        fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
        fprintf(output_stream, "Days: %d\n", calendar->days);
        fprintf(output_stream, "Total Events: %d\n", calendar->total_events);
    }
    fprintf(output_stream, "\n**** Events ****\n");
    
    if (calendar->total_events > 0){
        for (i = 0; i < calendar->days; i++){
            fprintf(output_stream, "Day %d\n", (i + 1));
            event = calendar->events[i];
            while (event){
                fprintf(output_stream, "Event's Name: \"%s\", Start_time: %d, Duration: %d\n", event->name, event->start_time, event->duration_minutes);
                event = event->next;
            }
        }
    }
    
    return SUCCESS;
}

/* Add the specified event to the list associated with the day parameter
   Notice that events are uniquely identified across the calendar by name */
int add_event(Calendar *calendar, const char *name, int start_time, int duration_minutes, void *info, int day){
    Event *event, *pre_event, *new_event;
    
    if (!calendar || !name || start_time < 0 || start_time > 2400 || duration_minutes <= 0 || day < 1 || day > calendar->days){
        return FAILURE;
    }
    
    if (find_event(calendar, name, &new_event) == FAILURE){
        new_event = malloc(sizeof(Event));
        if (!new_event){
            return FAILURE;
        }
        new_event->name = (char *)malloc(strlen(name) + 1);
        if (!new_event->name){
            return FAILURE;
        }
        new_event->duration_minutes = duration_minutes;
        new_event->info = info;
        strcpy(new_event->name, name);
        new_event->start_time = start_time;
        
        event = calendar->events[day - 1];
        
        /* if there is no events in this lane */
        if (!event){
            calendar->events[day - 1] = new_event;
            new_event->next = NULL;
        }else{
            pre_event = event;
            /* Add at the beginning */
            if (calendar->comp_func(event, new_event) > 0){
                calendar->events[day - 1] = new_event;
                new_event->next = event;
            }
            /* Add at the middle or the end */
            else{
                pre_event = event;
                event = event->next;
                while (event && calendar->comp_func(event, new_event) <= 0){
                    pre_event = event;
                    event = event->next;
                }
                new_event->next = event;
                pre_event->next = new_event;
            }
        }
        (calendar->total_events)++;
    }else{
        return FAILURE;
    }
    
    return SUCCESS;
}

/* Return a pointer (via the out parameter event) to the event with the specified name (if any) */
int find_event(Calendar *calendar, const char *name, Event **event){
    Event *event_ptr;
    int day;
    
    if (calendar && name){
        for (day = 0; day < calendar->days; day++){
            event_ptr = calendar->events[day];
            while (event_ptr){
                if (strcmp(name, event_ptr->name) == 0 && event){
                    *event = event_ptr;
                    return SUCCESS;
                }
                event_ptr = event_ptr->next;
            }
        }
    }
    
    return FAILURE;
}

/* Return a pointer (via the out parameter event) to the event with the specified name in the specified day (if such event exist) */
int find_event_in_day(Calendar *calendar, const char *name, int day, Event **event){
    Event *event_ptr;
    
    if (calendar && name && day >= 1 && day <= calendar->days){
        event_ptr = calendar->events[day - 1];
        while (event_ptr){
            if (strcmp(name, event_ptr->name) == 0 && event){
                *event = event_ptr;
                return SUCCESS;
            }
            event_ptr = event_ptr->next;
        }
    }
    
    return FAILURE;
}

/* Remove the specified event from the calendar returning any memory allocated for the event */
int remove_event(Calendar *calendar, const char *name){
    Event *event, *pre_event;
    int day;
    
    if (!calendar || !name || find_event(calendar, name, &event) == FAILURE){
        return FAILURE;
    }
    
    for (day = 0; day < calendar->days; day++){
        event = calendar->events[day];
        pre_event = event;
        
        if (event){
            /* Remove the first event */
            if (strcmp(event->name, name) == 0){
                calendar->events[day] = event->next;
                event->next = NULL;
            }
            /* Remove from the middle or end */
            else{
                while (event && strcmp(event->name, name) != 0){
                    pre_event = event;
                    event = event->next;
                }
                pre_event->next = event->next;
                event->next = NULL;
            }
        free(event->name);
        if (calendar->free_info_func) {
            calendar->free_info_func(event->info);
        }
        free(event);
        (calendar->total_events)--;
        }
    }
    
    return SUCCESS;
}

/* Returns the info pointer associated with the specified event */
void *get_event_info(Calendar *calendar, const char *name){
    Event *event;
    
    if (find_event(calendar, name, &event) == FAILURE){
        return NULL;
    }
    return event->info;
}

/* Remove all the event lists associated with the calendar and set them to empty lists */
int clear_calendar(Calendar *calendar){
    int day;
    
    if (!calendar){
        return FAILURE;
    }
    
    for (day = 0; day < calendar->days; day++){
        clear_day(calendar, day + 1);
    }
    
    return SUCCESS;
}

/* Remove all the events for the specified day setting the event list to empty */
int clear_day(Calendar *calendar, int day){
    Event *event, *pre_event;
    
    if (!calendar || day < 1 || day > calendar->days){
        return FAILURE;
    }
    
    event = calendar->events[day - 1];
    while (event){
        pre_event = event;
        event = event->next;
        
        free(pre_event->name);
        if (pre_event->info && calendar->free_info_func){
            calendar->free_info_func(pre_event->info);
        }
        free(pre_event);
        
        calendar->total_events--;
    }
    
    calendar->events[day - 1] = NULL;
    
    return SUCCESS;
}

/* Return memory that was dynamically-allocated for the calendar */
int destroy_calendar(Calendar *calendar){
    if (!calendar){
        return FAILURE;
    }
    
    free(calendar->name);
    clear_calendar(calendar);
    free(calendar->events);
    free(calendar);
    
    return SUCCESS;
}
