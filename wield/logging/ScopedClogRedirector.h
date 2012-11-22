#pragma once
#include <iostream>
#include <fstream>

#include <wield/platform/string>

namespace wield { namespace logging {

    //! example scoped_clog_redirect<std::stringstream> for capturing logging in a std::stringstream
    template<typename OStreamType>
    class ScopedClogRedirector
    {
    public:
        ScopedClogRedirector()
            : ostream_()
            , old_clog_(std::clog.rdbuf(ostream_.rdbuf()))
        {
        }

        ~ScopedClogRedirector()
        {
            // restore the old logger
            std::clog.rdbuf(old_clog_);
        }

    private:
        OStreamType ostream_;
        std::streambuf* old_clog_;
    };

    //! specialization for using std::ostringstream
    template<>
    class ScopedClogRedirector<std::ostringstream>
    {
    public:
        ScopedClogRedirector()
            : ostream_()
            , old_clog_(std::clog.rdbuf(ostream_.rdbuf()))
        {
        }

        ~ScopedClogRedirector()
        {
            // restore the old logger
            std::clog.rdbuf(old_clog_);
        }

        std::string str(void) const
        {
            return ostream_.str();
        }

    private:
        std::ostringstream ostream_;
        std::streambuf* old_clog_;
    };


    // specialization for redirecting std::clog to a log file.
    template<>
    class ScopedClogRedirector<std::ofstream>
    {
    public:
        ScopedClogRedirector(const std::string& logPath)
            : logPath_(logPath)
            , file_stream_(logPath, std::ios_base::out | std::ios_base::app | std::ios_base::ate)
            , old_clog_(std::clog.rdbuf(file_stream_.rdbuf()))
        {
        }

        ~ScopedClogRedirector()
        {
            // restore the original clog
            std::clog.rdbuf(old_clog_);
        }

        const std::string& logFile(void) const
        {
            return logPath_;
        }

    private:
        std::string logPath_;
        std::ofstream file_stream_;
        std::streambuf* old_clog_;
    }; 

}}

