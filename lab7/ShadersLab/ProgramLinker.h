#pragma once
#include "pch.h"
#include "ProgramBase.h"

class ProgramLinker
{
public:
    ProgramLinker(void) {}
    ~ProgramLinker(void) {}

    void LinkProgram(GLuint program) 
    {
        ProgramHandle prog(program);
        prog.Link();
        m_programs.push_back(program);
    }

    void CheckStatus() 
    {
        std::stringstream strm;

        bool hasErrors = false;

        for (size_t i = 0; i < m_programs.size(); ++i)
        {
            ProgramHandle program(m_programs[i]);
            if (program.GetParameter(GL_LINK_STATUS) != GL_TRUE)
            {
                hasErrors = true;
                strm << "Program " << program << " linkage failed: " <<
                    program.GetInfoLog() << "\n";
            }
        }

        m_programs.clear();

        if (hasErrors)
        {
            throw std::runtime_error(strm.str());
        }
    }

private:
    std::vector<GLuint> m_programs;
};