/*
**********************************************************************
* Copyright (c) 2003-2007, International Business Machines
* Corporation and others.  All Rights Reserved.
**********************************************************************
* Author: Alan Liu
* Created: September 24 2003
* Since: ICU 2.8
**********************************************************************
*/
#include "ruleiter.h"
#include "unicode/parsepos.h"
#include "unicode/unistr.h"
#include "unicode/symtable.h"
#include "util.h"

/* \U87654321 or \ud800\udc00 */
#define MAX_U_NOTATION_LEN 12

U_NAMESPACE_BEGIN

RuleCharacterIterator::RuleCharacterIterator(const UnicodeString& theText, const SymbolTable* theSym,
                      ParsePosition& thePos) :
    text(theText),
    pos(thePos),
    sym(theSym),
    buf(0),
    bufPos(0)
{}

UBool RuleCharacterIterator::atEnd() const {
    return buf == 0 && pos.getIndex() == text.length();
}

UChar32 RuleCharacterIterator::next(int32_t options, UBool& isEscaped, UErrorCode& ec) {
    if (U_FAILURE(ec)) return DONE;

    UChar32 c = DONE;
    isEscaped = FALSE;

    for (;;) {
        c = _current();
        _advance(UTF_CHAR_LENGTH(c));

        if (c == SymbolTable::SYMBOL_REF && buf == 0 &&
            (options & PARSE_VARIABLES) != 0 && sym != 0) {
            