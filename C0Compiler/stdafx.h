// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <set>
#include <vector>

#include "debugutils.h"
#include "token.h"
#include "lexical_error.h"
#include "lexer.h"
#include "dataset.h"

#include "fastgraph.h"
#include "regex_handler.h"

#include "symbol.h"
#include "symbol_table.h"

typedef hash_map<Symbol*, int> reg_map;
typedef hash_map<Symbol*, int> mem_map;

#include "quaterion.h"
#include "quaterion_table.h"
#include "syntax_error.h"
#include "syntax.h"
#include "mips_data.h"
#include "mips_code.h"
#include "mips_table.h"

#include "basicblock.h"
#include "dnode.h"
#include "dag.h"
#include "duchain.h"
#include "regdistributor.h"
#include "flow.h"







// TODO: 在此处引用程序需要的其他头文件
