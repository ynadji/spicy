// Copyright (c) 2020 by the Zeek Project. See LICENSE for details.

#pragma once

namespace hilti::rt {

struct Context;

/**
 * Initializes the HILTI run-time library. This must be called once at
 * startup before any other libhilti functionality can be used. The one
 * exception to that rule `hlt_set_config()`, which can (and must) be called
 * before hlt_init().
 */
extern void init();

/**
 * Shuts down the run-time library, freeing all resources. Once executed, no
 * libhilti functioality can be used anymore.
 */
extern void done();

/** Returns true if init() has already been called. */
extern bool isInitialized();

namespace detail {

/** A HILTI module registered with the runtime. The HILTI code generator creates code to register an instance of this
 * for every module it compiles. */
struct HiltiModule {
    const char* name;      /**< name of the HILTI module; for informational purposes */
    void (*init_module)(); /**< callback for executing any top-level module code when the runtime library is being
                              initialized */
    void (*init_globals)(
        hilti::rt::Context* ctx); /**< callback to initialize the module's globals in a freshly allocatec context */
    unsigned int*
        globals_idx; /**< pointer to an integer storing the modules' index in the context-wide globals array */
};

/** Entry point for the generated code to register a compiled HILTI module with the runtime */
extern void registerModule(HiltiModule module);

/** Macro to schedule a global function to be called at startup time through a global constructor. */
#define HILTI_PRE_INIT(func) static ::hilti::rt::detail::ExecutePreInit __pre_init_##__COUNTER__(func);

/** Helper class to execute a global function at startup time through a global constructor. */
class ExecutePreInit {
public:
    ExecutePreInit(void (*f)()) { (*f)(); }
};

} // namespace detail

} // namespace hilti::rt
