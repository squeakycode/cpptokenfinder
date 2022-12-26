#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include <cpptokenfinder/cpptokenfinder.hpp>
#include <string>


class token_finder_test_comparer
{
public:
    bool operator()(char character_of_token, char character_of_searched_text) const
    {
        if (character_of_token == '?' && character_of_searched_text >= '0' && character_of_searched_text <= '9')
        {
            return true;
        }
        else
        {
            return character_of_token == character_of_searched_text;
        }
    }
};

TEST_CASE("token_finder", "[token_finder]")
{
    typedef cpptokenfinder::token_finder<char, int, int, -1> token_finder_type;

    SECTION("Create and Destroy")
    {
        token_finder_type tokenfinder;
    }

    SECTION("Invalid Arguments")
    {
        token_finder_type tokenfinder;
        CHECK_THROWS_AS(tokenfinder.add_token((const char *) nullptr, 0), std::invalid_argument);
        CHECK_THROWS_AS(tokenfinder.add_token("token", -1), std::invalid_argument);
        CHECK_THROWS_AS(tokenfinder.add_token("", 0), std::invalid_argument);
        CHECK_THROWS_AS(tokenfinder.add_token(std::string(), 0), std::invalid_argument);
        tokenfinder.add_token("token", 0);
        CHECK_THROWS_AS(tokenfinder.add_token("token", 0), std::invalid_argument);
    }

    SECTION("Single Token")
    {
        const char *text1 = "token";  // full match
        const char *text2 = "atoken"; // at end
        const char *text3 = "tokenb"; // at begin
        const char *text4 = "atokenb"; // infix
        const char *text5 = ""; // empty
        const char *text6 = "xyz"; // not match
        const char *text7 = "tokea"; // partial

        {
            token_finder_type tokenfinder;
            tokenfinder.add_token("token", 23);

            const char *token_begin = nullptr;
            const char *token_end = nullptr;
            int tokenid = 0;

            CHECK(tokenfinder.find_token(text1, token_begin, token_end, tokenid));
            CHECK(token_begin == text1);
            CHECK(token_end == text1 + 5);
            CHECK(tokenid == 23);
        }

        {
            token_finder_type tokenfinder;
            tokenfinder.add_token("token", 2);

            const char *token_begin = nullptr;
            const char *token_end = nullptr;
            int tokenid = 0;

            CHECK(tokenfinder.find_token(text2, token_begin, token_end, tokenid));
            CHECK(token_begin == text2 + 1);
            CHECK(token_end == text2 + 6);
            CHECK(tokenid == 2);
        }

        {
            token_finder_type tokenfinder;
            tokenfinder.add_token("token", 2);

            const char *token_begin = nullptr;
            const char *token_end = nullptr;
            int tokenid = 0;

            CHECK(tokenfinder.find_token(text3, token_begin, token_end, tokenid));
            CHECK(token_begin == text3);
            CHECK(token_end == text3 + 5);
            CHECK(tokenid == 2);
        }

        {
            token_finder_type tokenfinder;
            tokenfinder.add_token("token", 2);

            const char *token_begin = nullptr;
            const char *token_end = nullptr;
            int tokenid = 0;

            CHECK(tokenfinder.find_token(text4, token_begin, token_end, tokenid));
            CHECK(token_begin == text4 + 1);
            CHECK(token_end == text4 + 6);
            CHECK(tokenid == 2);
        }

        {
            token_finder_type tokenfinder;
            tokenfinder.add_token("token", 2);

            const char *token_begin = nullptr;
            const char *token_end = nullptr;
            int tokenid = 0;

            CHECK(!tokenfinder.find_token(text5, token_begin, token_end, tokenid));
            CHECK(token_begin == nullptr);
            CHECK(token_end == nullptr);
            CHECK(tokenid == 0);
        }

        {
            token_finder_type tokenfinder;
            tokenfinder.add_token("token", 2);

            const char *token_begin = nullptr;
            const char *token_end = nullptr;
            int tokenid = 0;

            CHECK(!tokenfinder.find_token(text6, token_begin, token_end, tokenid));
            CHECK(token_begin == nullptr);
            CHECK(token_end == nullptr);
            CHECK(tokenid == 0);
        }

        {
            token_finder_type tokenfinder;
            tokenfinder.add_token("token", 2);

            const char *token_begin = nullptr;
            const char *token_end = nullptr;
            int tokenid = 0;

            CHECK(!tokenfinder.find_token(text7, token_begin, token_end, tokenid));
            CHECK(token_begin == nullptr);
            CHECK(token_end == nullptr);
            CHECK(tokenid == 0);
        }
    }

    SECTION("Single Char Token")
    {
        token_finder_type tokenfinder;
        tokenfinder.add_token("a", 2);

        const char *text = "a aa";

        const char *token_begin = nullptr;
        const char *token_end = nullptr;
        int tokenid = 0;

        CHECK(tokenfinder.find_token(text, token_begin, token_end, tokenid));
        CHECK(token_begin == text);
        CHECK(token_end == text + 1);
        CHECK(tokenid == 2);

        CHECK(tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 2);
        CHECK(token_end == text + 3);
        CHECK(tokenid == 2);

        CHECK(tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 3);
        CHECK(token_end == text + 4);
        CHECK(tokenid == 2);
    }


    SECTION("Two Tokens disjunct")
    {
        token_finder_type tokenfinder;
        tokenfinder.add_token("token", 2);
        tokenfinder.add_token("xyz", 3);

        const char *text = "A token xyz.";

        const char *token_begin = nullptr;
        const char *token_end = nullptr;
        int tokenid = 0;

        CHECK(tokenfinder.find_token(text, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 2);
        CHECK(token_end == text + 7);
        CHECK(tokenid == 2);

        CHECK(tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 8);
        CHECK(token_end == text + 11);
        CHECK(tokenid == 3);

        CHECK(!tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 8); //does not change
        CHECK(token_end == text + 11); //does not change
        CHECK(tokenid == 3); //does not change
    }

    SECTION("Two Tokens, one is prefix of other token")
    {
        token_finder_type tokenfinder;
        tokenfinder.add_token("token", 2);
        tokenfinder.add_token("tok", 3);

        const char *text = "A token tok.";

        const char *token_begin = nullptr;
        const char *token_end = nullptr;
        int tokenid = 0;

        CHECK(tokenfinder.find_token(text, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 2);
        CHECK(token_end == text + 7);
        CHECK(tokenid == 2);

        CHECK(tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 8);
        CHECK(token_end == text + 11);
        CHECK(tokenid == 3);

        CHECK(!tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 8); //does not change
        CHECK(token_end == text + 11); //does not change
        CHECK(tokenid == 3); //does not change
    }

    SECTION("Two Tokens, one is infix of other token")
    {
        token_finder_type tokenfinder;
        tokenfinder.add_token("token", 2);
        tokenfinder.add_token("oke", 3);

        const char *text = "A token oke.";

        const char *token_begin = nullptr;
        const char *token_end = nullptr;
        int tokenid = 0;

        CHECK(tokenfinder.find_token(text, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 2);
        CHECK(token_end == text + 7);
        CHECK(tokenid == 2);

        CHECK(tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 8);
        CHECK(token_end == text + 11);
        CHECK(tokenid == 3);

        CHECK(!tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 8); //does not change
        CHECK(token_end == text + 11); //does not change
        CHECK(tokenid == 3); //does not change
    }

    SECTION("String type iterator, not const")
    {
        token_finder_type tokenfinder;
        tokenfinder.add_token("token", 2);

        std::string text = "a tokenfinder finds tokens";

        std::string::iterator token_begin;
        std::string::iterator token_end;
        int tokenid = 0;

        CHECK(tokenfinder.find_token(text.begin(), text.end(), token_begin, token_end, tokenid));
        CHECK(tokenfinder.find_token(token_end, text.end(), token_begin, token_end, tokenid));
        CHECK(!tokenfinder.find_token(token_end, text.end(), token_begin, token_end, tokenid));
    }

    SECTION("Char type iterator, not const")
    {
        token_finder_type tokenfinder;
        tokenfinder.add_token("token", 2);

        char buffer[] = {"a tokenfinder finds tokens"};

        char *token_begin = nullptr;
        char *token_end = nullptr;
        int tokenid = 0;

        CHECK(tokenfinder.find_token(buffer, token_begin, token_end, tokenid));
        CHECK(tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(!tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
    }

    SECTION("Char type iterator, wchar_t")
    {
        typedef cpptokenfinder::token_finder<wchar_t, int, int, -1> token_finderw_type;
        token_finderw_type tokenfinder;
        tokenfinder.add_token(L"token", 2);

        wchar_t buffer[] = {L"a tokenfinder finds tokens"};

        wchar_t *token_begin = nullptr;
        wchar_t *token_end = nullptr;
        int tokenid = 0;

        CHECK(tokenfinder.find_token(buffer, token_begin, token_end, tokenid));
        CHECK(tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(!tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
    }

    SECTION("Char type iterator, const wchar_t")
    {
        typedef cpptokenfinder::token_finder<wchar_t, int, int, -1> token_finderw_type;
        token_finderw_type tokenfinder;
        tokenfinder.add_token(L"token", 2);

        const wchar_t *text = L"a tokenfinder finds tokens";

        const wchar_t *token_begin = nullptr;
        const wchar_t *token_end = nullptr;
        int tokenid = 0;

        CHECK(tokenfinder.find_token(text, token_begin, token_end, tokenid));
        CHECK(tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(!tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
    }

    SECTION("Exchange comparer")
    {
        typedef cpptokenfinder::token_finder<char, int, int, -1, token_finder_test_comparer> token_finderp_type;
        token_finderp_type tokenfinder;

        tokenfinder.add_token("machine???", 0);
        tokenfinder.add_token("port????", 1);

        const char *text = "The machine234 is connected to port1233 via network to machine2.";

        const char *token_begin = nullptr;
        const char *token_end = nullptr;
        int tokenid = 0;

        CHECK(tokenfinder.find_token(text, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 4);
        CHECK(token_end == text + 14);
        CHECK(tokenid == 0);

        CHECK(tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
        CHECK(token_begin == text + 31);
        CHECK(token_end == text + 39);
        CHECK(tokenid == 1);

        CHECK(!tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
    }

    SECTION("Clear")
    {
        token_finder_type tokenfinder;
        tokenfinder.add_token("token", 2);

        char buffer[] = {"a tokenfinder finds tokens"};

        char *token_begin;
        char *token_end;
        int tokenid = 0;

        CHECK(tokenfinder.find_token(buffer, token_begin, token_end, tokenid));
        tokenfinder.clear();
        CHECK(!tokenfinder.find_token(buffer, token_begin, token_end, tokenid));
    }
}

TEST_CASE("token_finder multiple tokens", "[token_finder]")
{
    typedef cpptokenfinder::token_finder<char, int, int, -1> token_finder_type;

    const char* testtokens[] =
    {
        "asm",
        "auto",
        "break",
        "case",
        "catch",
        "char",
        "class",
        "const",
        "continue",
        "default",
        "delete",
        "do",
        "double",
        "else",
        "enum",
        "extern",
        "float",
        "for",
        "fritoken_end",
        "goto",
        "if",
        "inline",
        "int",
        "long",
        "new",
        "operator",
        "private",
        "protected",
        "public",
        "register",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "struct",
        "switch",
        "template",
        "this",
        "throw",
        "try",
        "typedef",
        "union",
        "unsigned",
        "virtual",
        "void",
        "volatile",
        "while",
        "a",
        "b",
        "c"
    };

    SECTION("Multiple Keywords const char* input")
    {
        token_finder_type tokenfinder;

        for (int i = 0; i < static_cast<int>(sizeof(testtokens) / sizeof(testtokens[0])); ++i)
        {
            tokenfinder.add_token(testtokens[i], i);
        }

        for (int i = 0; i < static_cast<int>(sizeof(testtokens) / sizeof(testtokens[0])); ++i)
        {
            std::string text = "zzz";
            text += testtokens[i];
            text += "zzz";

            const char* token_begin = nullptr;
            const char* token_end = nullptr;
            int tokenid = 0;

            std::string token = testtokens[i];

            CHECK(tokenfinder.find_token(text.c_str(), token_begin, token_end, tokenid));
            CHECK(!tokenfinder.find_token(token_end, token_begin, token_end, tokenid));
            CHECK(token_begin == text.c_str() + 3);
            CHECK(token_end == text.c_str() + 3 + token.size());
            CHECK(tokenid == i);
        }
    }

    SECTION("Multiple Keywords std::string")
    {
        token_finder_type tokenfinder;

        for (int i = 0; i < static_cast<int>(sizeof(testtokens) / sizeof(testtokens[0])); ++i)
        {
            tokenfinder.add_token(std::string(testtokens[i]), i);
        }

        for (int i = 0; i < static_cast<int>(sizeof(testtokens) / sizeof(testtokens[0])); ++i)
        {
            std::string text = "zzz";
            text += testtokens[i];
            text += "zzz";

            std::string::const_iterator token_begin;
            std::string::const_iterator token_end;
            int tokenid = 0;

            std::string token = testtokens[i];

            CHECK(tokenfinder.find_token(text, token_begin, token_end, tokenid));
            CHECK(!tokenfinder.find_token(token_end, text.cend(), token_begin, token_end, tokenid));
            CHECK(token_begin == text.cbegin() + 3);
            CHECK(token_end == text.cbegin() + 3 + token.size());
            CHECK(tokenid == i);
        }
    }
}
