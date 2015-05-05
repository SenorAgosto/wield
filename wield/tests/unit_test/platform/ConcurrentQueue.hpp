#pragma once

#ifdef _WIN32

    // Use Microsoft's concurrent_queue implementation
    // Disable some warnings
    #pragma warning(push)
    #pragma warning(disable : 4127 4625 4626)
    #include <concurrent_queue.h>
    #pragma warning(pop)
#else
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    #pragma clang diagnostic ignored "-Wsign-conversion"
    // Use Intel Thread Build Blocks concurrent_queue
    #include <tbb/concurrent_queue.h>
    namespace Concurrency = tbb::strict_ppl;
    #pragma clang diagnostic pop 
#endif
