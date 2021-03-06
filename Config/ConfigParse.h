#ifndef CFGPARSE_H
#define CFGPARSE_H


#include <fstream>
#include <iostream>

#include "ConfigGlobals.h"

class CfgParse
{
    public:
        CfgParse(const char* path);
        CfgParse();
        virtual ~CfgParse();

        //////////////////////////////////////
        /// Loads the config file
        //////////////////////////////////////
        bool Load(const char* path);
        bool Load();

        //////////////////////////////////////
        /// This converts the loaded file
        /// into a CfgObject tree
        //////////////////////////////////////
        //Config& Parse(Config& data);

        ///Returns true when ready for a parse
        bool IsReady();

        bool IsParsed(){return m_parsed;};

        void TrimString(std::string& to_trim, bool left=true, bool right=true);
        void ToLower(std::string& str);
        bool StringIsInt(std::string str);
    protected:
    private:
        const char* m_path;
        bool m_parsed;
        std::ifstream m_config_file;
};

#endif // CFGPARSE_H
