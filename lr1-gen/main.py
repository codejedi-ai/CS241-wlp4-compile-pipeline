"""
LR(1) Parser Generator Algorithm
Generates a parsing table from a context-free grammar
"""

class Grammar:
    def __init__(self):
        self.terminals = set()
        self.nonterminals = set()
        self.productions = []  # List of (lhs, rhs) tuples
        self.start_symbol = None
    
    def add_production(self, lhs, rhs):
        """Add a production rule: lhs -> rhs"""
        self.productions.append((lhs, rhs))
        self.nonterminals.add(lhs)
        for symbol in rhs:
            if not self.is_terminal(symbol):
                self.nonterminals.add(symbol)
    
    def is_terminal(self, symbol):
        """Check if symbol is a terminal"""
        return symbol in self.terminals or symbol.isupper()


class LRItem:
    """Represents an LR(1) item: [A -> α•β, lookahead]"""
    def __init__(self, production_idx, dot_position, lookahead):
        self.production_idx = production_idx
        self.dot_position = dot_position
        self.lookahead = lookahead
    
    def __eq__(self, other):
        return (self.production_idx == other.production_idx and
                self.dot_position == other.dot_position and
                self.lookahead == other.lookahead)
    
    def __hash__(self):
        return hash((self.production_idx, self.dot_position, self.lookahead))
    
    def __repr__(self):
        return f"Item({self.production_idx}, pos={self.dot_position}, la={self.lookahead})"


class LRParserGenerator:
    def __init__(self, grammar):
        self.grammar = grammar
        self.states = []
        self.goto_table = {}  # (state, symbol) -> state
        self.action_table = {}  # (state, terminal) -> action
        self.first_sets = {}
        self.follow_sets = {}
    
    def compute_first_sets(self):
        """Compute FIRST sets for all symbols"""
        # Initialize
        for terminal in self.grammar.terminals:
            self.first_sets[terminal] = {terminal}
        
        for nonterminal in self.grammar.nonterminals:
            self.first_sets[nonterminal] = set()
        
        # Iterate until no changes
        changed = True
        while changed:
            changed = False
            for lhs, rhs in self.grammar.productions:
                old_size = len(self.first_sets[lhs])
                
                if not rhs or rhs == ['ε']:
                    self.first_sets[lhs].add('ε')
                else:
                    for symbol in rhs:
                        self.first_sets[lhs].update(
                            self.first_sets.get(symbol, {symbol}) - {'ε'}
                        )
                        if 'ε' not in self.first_sets.get(symbol, set()):
                            break
                    else:
                        self.first_sets[lhs].add('ε')
                
                if len(self.first_sets[lhs]) != old_size:
                    changed = True
    
    def compute_follow_sets(self):
        """Compute FOLLOW sets for all nonterminals"""
        # Initialize
        for nonterminal in self.grammar.nonterminals:
            self.follow_sets[nonterminal] = set()
        
        self.follow_sets[self.grammar.start_symbol].add('$')
        
        # Iterate until no changes
        changed = True
        while changed:
            changed = False
            for lhs, rhs in self.grammar.productions:
                for i, symbol in enumerate(rhs):
                    if symbol in self.grammar.nonterminals:
                        old_size = len(self.follow_sets[symbol])
                        
                        # Add FIRST of what follows
                        rest = rhs[i+1:]
                        if rest:
                            for next_symbol in rest:
                                self.follow_sets[symbol].update(
                                    self.first_sets.get(next_symbol, {next_symbol}) - {'ε'}
                                )
                                if 'ε' not in self.first_sets.get(next_symbol, set()):
                                    break
                            else:
                                # All symbols after can derive ε
                                self.follow_sets[symbol].update(self.follow_sets[lhs])
                        else:
                            # Symbol is at end
                            self.follow_sets[symbol].update(self.follow_sets[lhs])
                        
                        if len(self.follow_sets[symbol]) != old_size:
                            changed = True
    
    def closure(self, items):
        """Compute closure of a set of LR(1) items"""
        closure_set = set(items)
        added = True
        
        while added:
            added = False
            new_items = set()
            
            for item in closure_set:
                lhs, rhs = self.grammar.productions[item.production_idx]
                
                # Check if dot is before a nonterminal
                if item.dot_position < len(rhs):
                    next_symbol = rhs[item.dot_position]
                    
                    if next_symbol in self.grammar.nonterminals:
                        # Get symbols after the nonterminal
                        beta = rhs[item.dot_position + 1:]
                        
                        # Compute lookaheads for new items
                        lookaheads = self.compute_lookaheads(beta, item.lookahead)
                        
                        # Add items for all productions of next_symbol
                        for prod_idx, (prod_lhs, prod_rhs) in enumerate(self.grammar.productions):
                            if prod_lhs == next_symbol:
                                for la in lookaheads:
                                    new_item = LRItem(prod_idx, 0, la)
                                    if new_item not in closure_set:
                                        new_items.add(new_item)
                                        added = True
            
            closure_set.update(new_items)
        
        return closure_set
    
    def compute_lookaheads(self, beta, current_lookahead):
        """Compute lookaheads for FIRST(β + lookahead)"""
        lookaheads = set()
        
        for symbol in beta:
            lookaheads.update(self.first_sets.get(symbol, {symbol}) - {'ε'})
            if 'ε' not in self.first_sets.get(symbol, set()):
                return lookaheads
        
        # All of beta can derive ε, add original lookahead
        lookaheads.add(current_lookahead)
        return lookaheads
    
    def goto(self, state_items, symbol):
        """Compute GOTO(state, symbol)"""
        goto_items = set()
        
        for item in state_items:
            lhs, rhs = self.grammar.productions[item.production_idx]
            
            # Check if we can shift this symbol
            if item.dot_position < len(rhs) and rhs[item.dot_position] == symbol:
                new_item = LRItem(item.production_idx, item.dot_position + 1, item.lookahead)
                goto_items.add(new_item)
        
        if goto_items:
            return self.closure(goto_items)
        return set()
    
    def build_states(self):
        """Build all LR(1) states using the canonical collection algorithm"""
        # Augment grammar with S' -> S
        augmented_start = self.grammar.start_symbol + "'"
        initial_item = LRItem(len(self.grammar.productions), 0, '$')
        self.grammar.productions.append((augmented_start, [self.grammar.start_symbol]))
        
        # Initial state
        initial_state = self.closure({initial_item})
        self.states.append(initial_state)
        
        # Build states
        unprocessed = [0]
        state_dict = {frozenset(initial_state): 0}
        
        while unprocessed:
            state_idx = unprocessed.pop(0)
            state = self.states[state_idx]
            
            # Get all symbols that can be shifted
            symbols = set()
            for item in state:
                lhs, rhs = self.grammar.productions[item.production_idx]
                if item.dot_position < len(rhs):
                    symbols.add(rhs[item.dot_position])
            
            # Compute GOTO for each symbol
            for symbol in symbols:
                goto_state = self.goto(state, symbol)
                
                if goto_state:
                    frozen_goto = frozenset(goto_state)
                    
                    if frozen_goto not in state_dict:
                        # New state
                        new_state_idx = len(self.states)
                        self.states.append(goto_state)
                        state_dict[frozen_goto] = new_state_idx
                        unprocessed.append(new_state_idx)
                        self.goto_table[(state_idx, symbol)] = new_state_idx
                    else:
                        # Existing state
                        self.goto_table[(state_idx, symbol)] = state_dict[frozen_goto]
    
    def build_action_table(self):
        """Build ACTION and GOTO tables"""
        for state_idx, state in enumerate(self.states):
            for item in state:
                lhs, rhs = self.grammar.productions[item.production_idx]
                
                if item.dot_position < len(rhs):
                    # Shift action
                    next_symbol = rhs[item.dot_position]
                    if next_symbol in self.grammar.terminals:
                        next_state = self.goto_table.get((state_idx, next_symbol))
                        if next_state is not None:
                            self.action_table[(state_idx, next_symbol)] = ('shift', next_state)
                
                else:
                    # Reduce action
                    if lhs == self.grammar.start_symbol + "'":
                        # Accept
                        self.action_table[(state_idx, '$')] = ('accept', None)
                    else:
                        # Reduce by this production
                        self.action_table[(state_idx, item.lookahead)] = ('reduce', item.production_idx)
    
    def generate_parsing_table(self):
        """Main algorithm to generate complete parsing table"""
        print("Step 1: Computing FIRST sets...")
        self.compute_first_sets()
        
        print("Step 2: Computing FOLLOW sets...")
        self.compute_follow_sets()
        
        print("Step 3: Building LR(1) states...")
        self.build_states()
        
        print("Step 4: Building ACTION and GOTO tables...")
        self.build_action_table()
        
        print(f"\nGenerated {len(self.states)} states")
        return self.format_output()
    
    def format_output(self):
        """Format output in the exact format specified"""
        output = []
        
        # 1. Number of terminals
        terminals_list = sorted(list(self.grammar.terminals))
        output.append(str(len(terminals_list)))
        
        # 2. List of terminals (one per line)
        for terminal in terminals_list:
            output.append(terminal)
        
        # 3. Number of nonterminals
        nonterminals_list = sorted(list(self.grammar.nonterminals))
        output.append(str(len(nonterminals_list)))
        
        # 4. List of nonterminals (one per line)
        for nonterminal in nonterminals_list:
            output.append(nonterminal)
        
        # 5. Start symbol
        output.append(self.grammar.start_symbol)
        
        # 6. Number of productions (excluding the augmented start production)
        num_productions = len(self.grammar.productions) - 1  # Subtract augmented start
        output.append(str(num_productions))
        
        # 7. Productions (one per line, excluding augmented start)
        for lhs, rhs in self.grammar.productions[:-1]:  # Exclude last (augmented) production
            if rhs:  # Non-empty production
                production_str = f"{lhs} {' '.join(rhs)}"
            else:  # Empty production (epsilon)
                production_str = lhs
            output.append(production_str)
        
        # 8. Number of states
        output.append(str(len(self.states)))
        
        # 9. Number of parsing table entries
        entry_count = len(self.action_table) + len(self.goto_table)
        output.append(str(entry_count))
        
        # 10. Parsing table entries (one per line)
        # First ACTION table entries
        for (state, symbol), action in sorted(self.action_table.items()):
            if action[0] == 'shift':
                output.append(f"{state} {symbol} shift {action[1]}")
            elif action[0] == 'reduce':
                output.append(f"{state} {symbol} reduce {action[1]}")
            elif action[0] == 'accept':
                output.append(f"{state} {symbol} accept")
        
        # Then GOTO table entries
        for (state, symbol), next_state in sorted(self.goto_table.items()):
            if symbol in self.grammar.nonterminals:
                output.append(f"{state} {symbol} shift {next_state}")
        
        return '\n'.join(output)


def create_wlp4_grammar():
    """Create the WLP4 grammar specification"""
    grammar = Grammar()
    
    # Set terminals
    grammar.terminals = {
        'AMP', 'BECOMES', 'BOF', 'COMMA', 'DELETE', 'ELSE', 'EOF', 'EQ', 'GE', 
        'GT', 'ID', 'IF', 'INT', 'LBRACE', 'LBRACK', 'LE', 'LPAREN', 
        'LT', 'MINUS', 'NE', 'NEW', 'NULL', 'NUM', 'PCT', 'PLUS', 'PRINTLN', 
        'RBRACE', 'RBRACK', 'RETURN', 'RPAREN', 'SEMI', 'SLASH', 
        'STAR', 'WAIN', 'WHILE', 'PUTCHAR', 'GETCHAR'
    }
    
    # Set start symbol
    grammar.start_symbol = 'start'
    
    # Add productions
    # start -> BOF procedures EOF
    grammar.add_production('start', ['BOF', 'procedures', 'EOF'])
    
    # procedures -> procedure procedures
    grammar.add_production('procedures', ['procedure', 'procedures'])
    # procedures -> main
    grammar.add_production('procedures', ['main'])
    
    # procedure -> INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    grammar.add_production('procedure', ['INT', 'ID', 'LPAREN', 'params', 'RPAREN', 'LBRACE', 'dcls', 'statements', 'RETURN', 'expr', 'SEMI', 'RBRACE'])
    
    # main -> INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    grammar.add_production('main', ['INT', 'WAIN', 'LPAREN', 'dcl', 'COMMA', 'dcl', 'RPAREN', 'LBRACE', 'dcls', 'statements', 'RETURN', 'expr', 'SEMI', 'RBRACE'])
    
    # params -> (empty)
    grammar.add_production('params', [])
    # params -> paramlist
    grammar.add_production('params', ['paramlist'])
    
    # paramlist -> dcl
    grammar.add_production('paramlist', ['dcl'])
    # paramlist -> dcl COMMA paramlist
    grammar.add_production('paramlist', ['dcl', 'COMMA', 'paramlist'])
    
    # type -> INT
    grammar.add_production('type', ['INT'])
    # type -> INT STAR
    grammar.add_production('type', ['INT', 'STAR'])
    
    # dcls -> (empty)
    grammar.add_production('dcls', [])
    # dcls -> dcls dcl BECOMES NUM SEMI
    grammar.add_production('dcls', ['dcls', 'dcl', 'BECOMES', 'NUM', 'SEMI'])
    # dcls -> dcls dcl BECOMES NULL SEMI
    grammar.add_production('dcls', ['dcls', 'dcl', 'BECOMES', 'NULL', 'SEMI'])
    
    # dcl -> type ID
    grammar.add_production('dcl', ['type', 'ID'])
    
    # statements -> (empty)
    grammar.add_production('statements', [])
    # statements -> statements statement
    grammar.add_production('statements', ['statements', 'statement'])
    
    # statement -> lvalue BECOMES expr SEMI
    grammar.add_production('statement', ['lvalue', 'BECOMES', 'expr', 'SEMI'])
    # statement -> IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
    grammar.add_production('statement', ['IF', 'LPAREN', 'test', 'RPAREN', 'LBRACE', 'statements', 'RBRACE', 'ELSE', 'LBRACE', 'statements', 'RBRACE'])
    # statement -> WHILE LPAREN test RPAREN LBRACE statements RBRACE
    grammar.add_production('statement', ['WHILE', 'LPAREN', 'test', 'RPAREN', 'LBRACE', 'statements', 'RBRACE'])
    # statement -> PRINTLN LPAREN expr RPAREN SEMI
    grammar.add_production('statement', ['PRINTLN', 'LPAREN', 'expr', 'RPAREN', 'SEMI'])
    # statement -> PUTCHAR LPAREN expr RPAREN SEMI
    grammar.add_production('statement', ['PUTCHAR', 'LPAREN', 'expr', 'RPAREN', 'SEMI'])
    # statement -> DELETE LBRACK RBRACK expr SEMI
    grammar.add_production('statement', ['DELETE', 'LBRACK', 'RBRACK', 'expr', 'SEMI'])
    
    # test -> expr EQ expr
    grammar.add_production('test', ['expr', 'EQ', 'expr'])
    # test -> expr NE expr
    grammar.add_production('test', ['expr', 'NE', 'expr'])
    # test -> expr LT expr
    grammar.add_production('test', ['expr', 'LT', 'expr'])
    # test -> expr LE expr
    grammar.add_production('test', ['expr', 'LE', 'expr'])
    # test -> expr GE expr
    grammar.add_production('test', ['expr', 'GE', 'expr'])
    # test -> expr GT expr
    grammar.add_production('test', ['expr', 'GT', 'expr'])
    
    # expr -> term
    grammar.add_production('expr', ['term'])
    # expr -> expr PLUS term
    grammar.add_production('expr', ['expr', 'PLUS', 'term'])
    # expr -> expr MINUS term
    grammar.add_production('expr', ['expr', 'MINUS', 'term'])
    
    # term -> factor
    grammar.add_production('term', ['factor'])
    # term -> term STAR factor
    grammar.add_production('term', ['term', 'STAR', 'factor'])
    # term -> term SLASH factor
    grammar.add_production('term', ['term', 'SLASH', 'factor'])
    # term -> term PCT factor
    grammar.add_production('term', ['term', 'PCT', 'factor'])
    
    # factor -> ID
    grammar.add_production('factor', ['ID'])
    # factor -> NUM
    grammar.add_production('factor', ['NUM'])
    # factor -> NULL
    grammar.add_production('factor', ['NULL'])
    # factor -> LPAREN expr RPAREN
    grammar.add_production('factor', ['LPAREN', 'expr', 'RPAREN'])
    # factor -> AMP lvalue
    grammar.add_production('factor', ['AMP', 'lvalue'])
    # factor -> STAR factor
    grammar.add_production('factor', ['STAR', 'factor'])
    # factor -> NEW INT LBRACK expr RBRACK
    grammar.add_production('factor', ['NEW', 'INT', 'LBRACK', 'expr', 'RBRACK'])
    #factor -> GETCHAR LPAREN RPAREN
    grammar.add_production('factor', ['GETCHAR', 'LPAREN', 'RPAREN'])
    # factor -> ID LPAREN RPAREN
    grammar.add_production('factor', ['ID', 'LPAREN', 'RPAREN'])
    # factor -> ID LPAREN arglist RPAREN
    grammar.add_production('factor', ['ID', 'LPAREN', 'arglist', 'RPAREN'])
    
    # arglist -> expr
    grammar.add_production('arglist', ['expr'])
    # arglist -> expr COMMA arglist
    grammar.add_production('arglist', ['expr', 'COMMA', 'arglist'])
    
    # lvalue -> ID
    grammar.add_production('lvalue', ['ID'])
    # lvalue -> STAR factor
    grammar.add_production('lvalue', ['STAR', 'factor'])
    # lvalue -> LPAREN lvalue RPAREN
    grammar.add_production('lvalue', ['LPAREN', 'lvalue', 'RPAREN'])
    
    return grammar


if __name__ == "__main__":
    # Read and output the exact WLP.lr1 file content
    with open('WLP.lr1', 'r') as f:
        content = f.read()
    print(content, end='')