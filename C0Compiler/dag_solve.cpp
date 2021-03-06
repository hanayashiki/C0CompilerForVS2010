#include "stdafx.h"

#define Q Quaterion

void DAG::extract_needed() {
	dataSet<Symbol*> & active_out = basic_block->active_out;
	for (hash_map<Symbol*, DNode*>::iterator
		sn_iter = sym_node_map.begin();
		sn_iter != sym_node_map.end();
		sn_iter++) {
		Symbol* sym = sn_iter->first;
		DNode* needed_node = sn_iter->second;
		if (in_mem(sym) && active_out.included(sym->id) ||
			sym->global) {
			set_needed(needed_node);
			coutd << "Set need: " << needed_node->idx << endl;
		}
		else {
			coutd << "Not needed: " << needed_node->idx << endl;
		}
	}
}

void DAG::set_needed(DNode* dn) {
	dn->needed = true;
	if (dn->left) {
		set_needed(dn->left);
	}
	if (dn->right) {
		set_needed(dn->right);
	}
}

void DAG::set_represent_sym(DNode* dn) {
	dataSet<Symbol*> & use = basic_block->use;
	dataSet<Symbol*> & active_out = basic_block->active_out;
	for (set<Symbol*>::iterator s_iter = dn->placed_syms.begin();
		s_iter != dn->placed_syms.end();
		s_iter++) {
		Symbol* sym = *s_iter;
		if (sym->const_flag) {
			// 常数必然是需要的
			// use 从基本块看来也是常数，因此也必须是需要的
			dn->calculated_sym = sym;
			return;
		}
	}
	for (set<Symbol*>::iterator s_iter = dn->placed_syms.begin();
		s_iter != dn->placed_syms.end();
		s_iter++) {
		Symbol* calculated = *s_iter;
		if (active_out.included(calculated->id)) { // 变量是否被后继需要
			dn->calculated_sym = calculated;
			return;
		}
	}
	// 如果都是不需要的，算第一个
	if (dn->placed_syms.size() > 0) {
		dn->calculated_sym = *(dn->placed_syms.begin());
	}
	else {
		dn->calculated_sym = NULL;
	}
}

void DAG::dump_quaterion() {
	extract_needed();
	for (vector<DNode*>::iterator d_iter = node_list.begin();
		d_iter != node_list.end(); d_iter++) {
		// Q new_q();
		DNode* dnode = *d_iter;
		if (dnode->calculated_sym == NULL) {
			set_represent_sym(dnode);
		}
		coutd << "examine node " << dnode->idx << endl;
		for (set<Symbol*>::iterator s_iter
			= dnode->placed_syms.begin();
			s_iter != dnode->placed_syms.end();
			s_iter++) {
			coutd << (*s_iter)->name << endl;
		}
		if (dnode->needed == false) {
			// 这个节点不需要
			coutd << "not needed\n";
			continue;
		}
		else if (dnode->op == Quaterion::SELF) {
			// 要做常量传播
			coutd << "propagated \n";
			overlap_propagation(dnode);
		}
		else if (dnode->fake) {
			coutd << "fake node \n";
			new_q_table.add_quaterion(dnode->q);
		}
		else {
			coutd << "added \n";
			Quaterion new_q;
			new_q.op = dnode->op;
			if (dnode->left) {
				new_q.left = dnode->left->calculated_sym;
			}
			if (dnode->right) {
				new_q.right = dnode->right->calculated_sym;
			}
			new_q.dst = dnode->calculated_sym;
			new_q.label = dnode->label;
			new_q_table.add_quaterion(new_q);

			overlap_propagation(dnode);
		}
	}
}

void DAG::overlap_propagation(DNode* dn) {
	dataSet<Symbol*> & active_out = basic_block->active_out;
	for (set<Symbol*>::iterator s_iter = dn->placed_syms.begin();
		s_iter != dn->placed_syms.end();
		s_iter++) {
		Symbol* sym = *s_iter;
		coutd << "propagating: " << sym->name << endl;
		if (sym != dn->calculated_sym) {
			if (in_mem(sym) && active_out.included(sym->id)
				|| sym->global) {
				if (sym_node_map[sym] == dn) {
					// 当前节点不是“最终”节点，这样的传播毫无意义
					// 因为上层节点不会引用 sym，sym在基本块中存储被
					// calculated_sym 所取代
					Quaterion new_q(sym, dn->calculated_sym);
					new_q_table.add_quaterion(new_q);
					coutd << "propagation succeed: " + sym->name << endl;
				}
			}
		}
		coutd << in_mem(sym) << (sym != dn->calculated_sym);

	}
}

