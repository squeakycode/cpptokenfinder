//-----------------------------------------------------------------------------
// cpptokenfinder
//-----------------------------------------------------------------------------

#include <cpptokenfinder/cpptokenfinder.hpp>
#include <iostream>
#include <string>

enum class TokenIds
{
    invalid,
    author,
    title
};

int main()
{
    typedef cpptokenfinder::token_finder<char, TokenIds, TokenIds, TokenIds::invalid> tokenfinder_t;

    tokenfinder_t tokenfinder;
    tokenfinder.add_token("@AUTHOR@", TokenIds::author);
    tokenfinder.add_token("@TITLE@", TokenIds::title);

    const struct
    {
        const char* title;
        const char* author;
    } books[] = { {"Title A", "Author A"}, {"Title B", "Author B"}, {"Title C", "Author C"} };

    const char* templatetext = "The book '@TITLE@' was written by @AUTHOR@.";

    for (const auto& book : books)
    {
        const char* tokenbegin = nullptr;
        const char* tokenend = nullptr;
        TokenIds tokenid = TokenIds::invalid;
        const char* text = templatetext;
        while (tokenfinder.find_token(text, tokenbegin, tokenend, tokenid))
        {
            // process text before the token
            std::cout.write(text, tokenbegin - text);
            
            // process the token
            if (tokenid == TokenIds::author)
            {
                std::cout << book.author;
            }
            else if (tokenid == TokenIds::title)
            {
                std::cout << book.title;
            }
            text = tokenend;
        }
        // process the trailing text if any
        if (text && *text)
        {
            std::cout << text;
        }
        std::cout << std::endl;
    }
}
