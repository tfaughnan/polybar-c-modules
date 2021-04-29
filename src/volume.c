/* volume.c */

#include <pulse/pulseaudio.h>

//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* TODO: logging function using pa_context_errno() ? */

/* TODO: variadic cleanup function might be nice */

/*
void cleanup(pa_mainloop *m, ...)
{
    va_list args;

}
*/

/* catch and handle SIGINT */
void sigint_cb(pa_mainloop_api *api, pa_signal_event *e, int sig, void *userdata)
{
    exit(EXIT_SUCCESS);
}

/* executes on sink events in our subscription */
void sink_info_cb(pa_context *c, const pa_sink_info *i, int eol, void *userdata)
{
    if (i)
    {
        /* TODO: print "MUTE" instead of volume if i->mute == 1 */

        pa_volume_t avgvol = pa_cvolume_avg(&i->volume);
        char avgvol_pct[PA_VOLUME_SNPRINT_MAX];
        pa_volume_snprint(avgvol_pct, PA_VOLUME_SNPRINT_MAX, avgvol);
        fprintf(stdout, "%s\n", avgvol_pct);
        fflush(stdout);

    }

}

/* executes on our context subscription events */
void ctx_subscribe_cb(pa_context *c, pa_subscription_event_type_t t, uint32_t idx, void *userdata)
{
    switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK)
    {
        case PA_SUBSCRIPTION_EVENT_SINK:
            pa_context_get_sink_info_by_index(c, idx, sink_info_cb, userdata);
            break;
        default:
            fprintf(stderr, "unexpected event type\n");
            break;
    }
}

/* executes once context is ready */
void ctx_ready_cb(pa_context *c, const pa_server_info *i, void *userdata)
{
    if (i)
    {
        pa_context_get_sink_info_by_name(c, i->default_sink_name, sink_info_cb, userdata);
    }
}

/* executes on changes to context state */
void ctx_state_cb(pa_context *c, void *userdata)
{
    pa_operation *op;

    pa_context_state_t state = pa_context_get_state(c);
    switch (state)
    {
        case PA_CONTEXT_READY:
            op = pa_context_get_server_info(c, ctx_ready_cb, userdata);
			if (op)
            {
                pa_operation_unref(op);
            }
            
            pa_context_set_subscribe_callback(c, ctx_subscribe_cb, userdata);
            op = pa_context_subscribe(c, PA_SUBSCRIPTION_MASK_SINK, NULL, userdata);
			if (op)
            {
                pa_operation_unref(op);
            }
            break;
        default:
            break;
    }
}

int main(int argc, char* argv[])
{
    /* create mainloop */
    pa_mainloop *mainloop = pa_mainloop_new();
    if (!mainloop)
    {
        fprintf(stderr, "pa_mainloop_new() failed\n");
        return EXIT_FAILURE;
    }

    /* get mainloop abstraction layer */
    pa_mainloop_api *mainloop_api = pa_mainloop_get_api(mainloop);
    if (!mainloop_api)
    {
        fprintf(stderr, "pa_mainloop_get_api() failed\n");
        pa_mainloop_free(mainloop);
        return EXIT_FAILURE;
    }

    /* initialize signal subsystem and bind to mainloop */
    if (pa_signal_init(mainloop_api) != 0)
    {
        fprintf(stderr, "pa_signal_init() failed\n");
        pa_mainloop_free(mainloop);
        return EXIT_FAILURE;
    }

    pa_signal_event *sigevt = pa_signal_new(SIGINT, sigint_cb, NULL);
    if (!sigevt)
    {
        fprintf(stderr, "pa_signal_init() failed\n");
        pa_signal_done();
        pa_mainloop_free(mainloop);
        return EXIT_FAILURE;
    }

    /* create context */
    pa_context *ctx = pa_context_new(mainloop_api, argv[0]);
    if (!ctx)
    {
        fprintf(stderr, "pa_context_new() failed\n");
        pa_signal_free(sigevt);
        pa_signal_done();
        pa_mainloop_free(mainloop);
        return EXIT_FAILURE;
    }

    /*  connect context to server (NULL means default server) */
    if (pa_context_connect(ctx, NULL, 0, NULL) < 0)
    {
        fprintf(stderr, "pa_context_connect() failed\n");
        pa_context_unref(ctx);
        pa_signal_free(sigevt);
        pa_signal_done();
        pa_mainloop_free(mainloop);
        return EXIT_FAILURE;
    }

    /* set callback function to execute on changes to context state */
    pa_context_set_state_callback(ctx, ctx_state_cb, mainloop);

    /* run mainloop and save its status on quit() */
    int retval = EXIT_SUCCESS;
    if (pa_mainloop_run(mainloop, &retval) < 0)
    {
        fprintf(stderr, "pa_mainloop_run() failed\n");
        return retval;
    }

    pa_context_unref(ctx);
    pa_signal_free(sigevt);
    pa_signal_done();
    pa_mainloop_free(mainloop);

    return retval;
}
