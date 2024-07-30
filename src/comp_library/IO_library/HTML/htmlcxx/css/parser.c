/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of htmlcxx -- A simple non-validating css1 and html parser
 * written in C++.
 *
 * htmlcxx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * htmlcxx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with htmlcxx.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright 2005-2010 Davi de Castro Reis and Robson Braga Araújo
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include "parser.h"

void free_rulesets(struct selector_list_t *rules) {

	struct selector_list_t *pos = rules;
	while (pos != NULL) {
		struct selector_list_t *tmp = pos;
		struct selector_t *rule = pos->selector;
		struct property_t *property = rule->property;
		while (property != NULL) {
			struct property_t *tmp = property;
			property = property->next;
			tmp->count--;
			if (tmp->count == 0) {
				free(tmp->name);
				free(tmp->val);
				free(tmp);
			}
		}
		while (rule != NULL) {
			struct selector_t *tmp = rule;
			rule = rule->next;
			free(tmp->element_name);
			free(tmp->id);
			free(tmp->e_class);
			free(tmp);
		}
		pos = pos->next;
		free(tmp);
	}

}
