/* LGPL */
#include <r_anal.h>
#include <r_types.h>
#include <r_cmd.h>
#include <r_core.h>

static void usage() {
        r_cons_printf("Usage: ahm CONST-NAME\n");
        r_cons_printf("To remove hint, use \"ah-\"\n");
}

static void _do_ahm(RAnal *anal, ut64 addr, const char *input) {
    switch (*input) {
    case ' ': {
        char *args = strdup(r_str_trim_ro(input));
        r_str_trim_tail(args);
        if (strchr(args, ' ')) {
            // only expecting one arg
            usage();
        } else {
            // TODO: check that the instruction at addr is elligible
            // for the offset analysis hint (has an immediate, etc)
            r_anal_hint_set_offset(anal, addr, args);
        }
        free(args);
        break;
    }
    default:
        usage();
    }
}

static int r_cmd_ahm(void *user, const char *input) {
    RCore *core = (RCore *) user;
    ut64 addr = core->offset;
    if (!strncmp (input, "ahm", 3)) {
        _do_ahm(core->anal, addr, input+3);
        return true;
    }
    return false;
}

RCorePlugin r_core_plugin_ahm = {
    .name = "Annotate magic constant",
    .desc = "Annotate magic constants in disassembly",
    .license = "LGPL-3.0",
    .call = r_cmd_ahm
};

#ifdef _MSC_VER
#define _R_API __declspec(dllexport)
#else
#define _R_API
#endif // MSC_VER

#ifndef CORELIB
_R_API RLibStruct radare_plugin = {
    .type = R_LIB_TYPE_CORE,
    .data = &r_core_plugin_ahm,
    .version = R2_VERSION
};
#endif
