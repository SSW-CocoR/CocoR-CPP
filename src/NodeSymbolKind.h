/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NodeSymbolKind.h
 * Author: mingo
 *
 * Created on July 14, 2022, 8:03 AM
 */

#ifndef NODESYMBOLKIND_H
#define NODESYMBOLKIND_H

// constants for node/symbol kinds
enum NodeType {
    id,
    t,      // terminal symbol
    pr,     // pragma
    nt,     // nonterminal symbol
    clas,   // character class
    chr,    // character
    wt,     // weak terminal symbol
    any,    //
    eps,    // empty
    nt_sync,   // synchronization symbol
    sem,    // semantic action: (. .)
    alt,    // alternative: |
    iter,   // iteration: { }
    opt,    // option: [ ]
    rslv,   // resolver expr
};
enum TransitionCode {
    normalTrans,		// transition codes
    contextTrans,
};


#endif /* NODESYMBOLKIND_H */

