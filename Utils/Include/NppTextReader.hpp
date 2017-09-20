#ifndef NPPTEXTREADER_HPP
#define NPPTEXTREADER_HPP

#include "ITextReader.hpp"
#include "EditorData.hpp"

namespace NppPlugin
{

class NppTextReader: public Plugin::ITextReader
{
public:
    NppTextReader(EditorData& p_npp);

    std::string getCurrentWord() const;

private:
	EditorData& m_npp;
};

} /* namespace NppPlugin */

#endif /* NPPTEXTREADER_H_ */
