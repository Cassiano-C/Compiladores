import re

# Definição das cores via constantes ANSI (Nativas do Python)
RESET = "\033[0m"
COR_NT = "\033[1;34m"        # Azul Negrito para Não-Terminais
COR_T = "\033[1;32m"         # Verde Negrito para Terminais e Tokens
COR_OP = "\033[1;31m"        # Vermelho Negrito para Símbolos Especiais e Épsilon
COR_SUCESSO = "\033[1;32m"   # Verde Claro para Sucesso
COR_ERRO = "\033[1;31m"      # Vermelho Claro para Erros e Conflitos

def eliminar_recursao_esquerda(gramatica):
    nao_terminais = list(gramatica.keys())
    nova_gramatica = {nt: [prod[:] for prod in gramatica[nt]] for nt in nao_terminais}

    for i in range(len(nao_terminais)):
        Ai = nao_terminais[i]
        for j in range(i):
            Aj = nao_terminais[j]
            novas_producoes_Ai = []
            for prod in nova_gramatica[Ai]:
                if prod and prod[0] == Aj:
                    alfa = prod[1:]
                    for prod_Aj in nova_gramatica[Aj]:
                        if prod_Aj == ['ε']:
                            novas_producoes_Ai.append(alfa if alfa else ['ε'])
                        else:
                            novas_producoes_Ai.append(prod_Aj + alfa)
                else:
                    novas_producoes_Ai.append(prod)
            nova_gramatica[Ai] = novas_producoes_Ai

        diretas = []
        nao_diretas = []
        for prod in nova_gramatica[Ai]:
            if prod and prod[0] == Ai:
                diretas.append(prod[1:])
            else:
                nao_diretas.append(prod)
                
        if diretas:
            Ai_linha = Ai + "'"
            while Ai_linha in nova_gramatica:
                Ai_linha += "'"
                
            nova_gramatica[Ai] = []
            nova_gramatica[Ai_linha] = []
            
            if not nao_diretas or nao_diretas == [['ε']]:
                nova_gramatica[Ai].append([Ai_linha])
            else:
                for beta in nao_diretas:
                    if beta == ['ε']:
                        nova_gramatica[Ai].append([Ai_linha])
                    else:
                        nova_gramatica[Ai].append(beta + [Ai_linha])
                        
            for alfa in diretas:
                nova_gramatica[Ai_linha].append(alfa + [Ai_linha])
            nova_gramatica[Ai_linha].append(['ε'])

    return nova_gramatica


def fatorar_esquerda(gramatica):
    """Remove prefixos comuns em produções do mesmo Não-Terminal."""
    nova_gramatica = {nt: [prod[:] for prod in prods] for nt, prods in gramatica.items()}
    mudou = True

    while mudou:
        mudou = False
        nao_terminais = list(nova_gramatica.keys())

        for nt in nao_terminais:
            producoes = nova_gramatica[nt]
            if len(producoes) <= 1:
                continue

            # Encontra o maior prefixo comum entre qualquer par de produções
            maior_prefixo = []
            for i in range(len(producoes)):
                for j in range(i + 1, len(producoes)):
                    p1, p2 = producoes[i], producoes[j]
                    min_len = min(len(p1), len(p2))
                    k = 0
                    while k < min_len and p1[k] == p2[k] and p1[k] != 'ε':
                        k += 1
                    if k > len(maior_prefixo):
                        maior_prefixo = p1[:k]

            # Se encontrou um prefixo comum válido
            if maior_prefixo:
                mudou = True
                nt_linha = nt + '"'
                while nt_linha in nova_gramatica:
                    nt_linha += '"'

                com_prefixo = []
                sem_prefixo = []

                for prod in producoes:
                    if prod[:len(maior_prefixo)] == maior_prefixo:
                        com_prefixo.append(prod[len(maior_prefixo):])
                    else:
                        sem_prefixo.append(prod)

                # Ajusta as produções do NT original e do novo NT"
                nova_gramatica[nt] = sem_prefixo + [maior_prefixo + [nt_linha]]
                nova_gramatica[nt_linha] = [p if p else ['ε'] for p in com_prefixo]
                break # Sai para reavaliar o dicionário atualizado

    return nova_gramatica


def obter_terminais_e_nao_terminais(gramatica):
    nao_terminais = set(gramatica.keys())
    terminais = set()
    for prods in gramatica.values():
        for prod in prods:
            for simbolo in prod:
                if simbolo != 'ε' and simbolo not in nao_terminais:
                    terminais.add(simbolo)
    return terminais, nao_terminais


def calcular_first(gramatica):
    terminais, nao_terminais = obter_terminais_e_nao_terminais(gramatica)
    first = {nt: set() for nt in nao_terminais}
    
    # FIRST de um terminal é ele mesmo
    for t in terminais:
        first[t] = {t}
    first['ε'] = {'ε'}

    mudou = True
    while mudou:
        mudou = False
        for nt, producoes in gramatica.items():
            for prod in producoes:
                tamanho_antigo = len(first[nt])
                
                if prod == ['ε']:
                    first[nt].add('ε')
                else:
                    for simbolo in prod:
                        # Se for um terminal, adiciona e para
                        if simbolo in terminais:
                            first[nt].add(simbolo)
                            break
                        # Se for Não-Terminal, adiciona o FIRST dele (exceto épsilon)
                        elif simbolo in nao_terminais:
                            first_simbolo = first[simbolo]
                            first[nt].update(first_simbolo - {'ε'})
                            # Se o FIRST do símbolo não contém épsilon, paramos aqui
                            if 'ε' not in first_simbolo:
                                break
                    else:
                        # Se o loop terminou sem 'break', significa que todos os
                        # símbolos da produção derivam épsilon. Logo, o NT deriva épsilon.
                        first[nt].add('ε')
                        
                if len(first[nt]) > tamanho_antigo:
                    mudou = True
    return {nt: first[nt] for nt in nao_terminais}


def calcular_follow(gramatica, first, simbolo_inicial):
    terminais, nao_terminais = obter_terminais_e_nao_terminais(gramatica)
    follow = {nt: set() for nt in nao_terminais}
    
    # Regra 1: Símbolo inicial recebe o cifrão '$' (fim da entrada)
    follow[simbolo_inicial].add('$')

    mudou = True
    while mudou:
        mudou = False
        for nt, producoes in gramatica.items():
            for prod in producoes:
                if prod == ['ε']:
                    continue
                
                for i, simbolo in enumerate(prod):
                    if simbolo in nao_terminais:
                        tamanho_antigo = len(follow[simbolo])
                        
                        # Espia o que vem DEPOIS do símbolo atual (beta)
                        resto = prod[i+1:]
                        
                        if resto:
                            # Regra 2: Adiciona o FIRST do resto (menos épsilon) ao FOLLOW do símbolo
                            for r_simbolo in resto:
                                if r_simbolo in terminais:
                                    follow[simbolo].add(r_simbolo)
                                    break
                                elif r_simbolo in nao_terminais:
                                    follow[simbolo].update(first[r_simbolo] - {'ε'})
                                    if 'ε' not in first[r_simbolo]:
                                        break
                            else:
                                # Se todo o resto pode virar épsilon, Regra 3 se aplica
                                follow[simbolo].update(follow[nt])
                        else:
                            # Regra 3: Se o símbolo é o último da produção, 
                            # FOLLOW(simbolo) recebe FOLLOW(nt_pai)
                            follow[simbolo].update(follow[nt])
                            
                        if len(follow[simbolo]) > tamanho_antigo:
                            mudou = True
    return follow

def calcular_first_da_sequencia(sequencia, first, terminais, nao_terminais):
    """Calcula o FIRST de uma cadeia de símbolos (uma produção específica)."""
    resultado = set()
    if not sequencia or sequencia == ['ε']:
        return {'ε'}
        
    for simbolo in sequencia:
        if simbolo in terminais:
            resultado.add(simbolo)
            break
        elif simbolo in nao_terminais:
            resultado.update(first[simbolo] - {'ε'})
            if 'ε' not in first[simbolo]:
                break
    else:
        # Se o loop terminou sem quebras, toda a sequência pode gerar épsilon
        resultado.add('ε')
        
    return resultado


def calcular_first_plus(gramatica, first, follow):
    """Calcula o FIRST+ para cada produção da gramática."""
    terminais, nao_terminais = obter_terminais_e_nao_terminais(gramatica)
    first_plus_gramatica = {}

    for nt, producoes in gramatica.items():
        first_plus_gramatica[nt] = []
        for prod in producoes:
            first_da_prod = calcular_first_da_sequencia(prod, first, terminais, nao_terminais)
            if 'ε' in first_da_prod:
                first_plus = (first_da_prod - {'ε'}) | follow[nt]
            else:
                first_plus = first_da_prod
                
            first_plus_gramatica[nt].append((prod, first_plus))
            
    return first_plus_gramatica


def verificar_se_eh_ll1(first_plus_gramatica):
    """Verifica se a gramática é LL(1)."""
    eh_ll1 = True
    conflitos = []

    for nt, lista_prods in first_plus_gramatica.items():
        if len(lista_prods) <= 1:
            continue
            
        for i in range(len(lista_prods)):
            for j in range(i + 1, len(lista_prods)):
                prod_a, f_plus_a = lista_prods[i]
                prod_b, f_plus_b = lista_prods[j]
                
                intersecao = f_plus_a.intersection(f_plus_b)
                
                if intersecao:
                    eh_ll1 = False
                    conflitos.append({
                        'nao_terminal': nt,
                        'prod1': prod_a,
                        'prod2': prod_b,
                        'simbolos_conflito': intersecao
                    })
                    
    return eh_ll1, conflitos


def imprimir_gramatica(gramatica):
    """Imprime a estrutura estruturada com cores inteligentes."""
    nao_terminais = set(gramatica.keys())
    for nt, prods in gramatica.items():
        linhas_coloridas = []
        for prod in prods:
            simbolos_coloridos = []
            for s in prod:
                if s in nao_terminais:
                    simbolos_coloridos.append(f"{COR_NT}{s}{RESET}")
                elif s == 'ε':
                    simbolos_coloridos.append(f"{COR_OP}ε{RESET}")
                else:
                    simbolos_coloridos.append(f"{COR_T}{s}{RESET}")
            linhas_coloridas.append(" ".join(simbolos_coloridos))
            
        print(f"{COR_NT}{nt}{RESET} -> {' | '.join(linhas_coloridas)}")


def imprimir_first_plus(first_plus, gramatica):
    """Exibe os conjuntos FIRST+ com destaque para regras e tokens."""
    nao_terminais = set(gramatica.keys())
    for nt, prods in first_plus.items():
        print(f"Não-Terminal: {COR_NT}{nt}{RESET}")
        for prod, f_plus in prods:
            prod_colorida = []
            for s in prod:
                if s in nao_terminais:
                    prod_colorida.append(f"{COR_NT}{s}{RESET}")
                elif s == 'ε':
                    prod_colorida.append(f"{COR_OP}ε{RESET}")
                else:
                    prod_colorida.append(f"{COR_T}{s}{RESET}")
            
            tokens_f_plus = ", ".join([f"{COR_T}{tk}{RESET}" for tk in sorted(list(f_plus))])
            print(f"  -> Regra: {COR_NT}{nt}{RESET} -> {' '.join(prod_colorida)} | {COR_OP}FIRST+{RESET} = [{tokens_f_plus}]")
        print(f"{COR_OP}-{RESET}" * 50)


if __name__ == "__main__":
    gramatica_linguagem = {
        'Program': [['FunctionList', 'EOF']],
        'FunctionList': [['Function', 'FunctionList'], ['ε']],
        'Function': [
            ['Type', 'ID', '(', 'ParamTypes', ')', '{', 'VarDeclBlock', 'StatementList', '}'],
            ['void', 'ID', '(', 'ParamTypes', ')', '{', 'VarDeclBlock', 'StatementList', '}']
        ],
        'VarDeclBlock': [['Type', 'VarDeclaration', 'VarDeclarationList', ';', 'VarDeclBlock'], ['ε']],
        'VarDeclarationList': [[',', 'VarDeclaration', 'VarDeclarationList'], ['ε']],
        'VarDeclaration': [['ID', 'OptionalArray']],
        'OptionalArray': [['[', 'integerconstant', ']'], ['ε']],
        'Type': [['char'], ['int']],
        'ParamTypes': [['void'], ['Type', 'ID', 'OptionalParamArray', 'ParamTypesList']],
        'OptionalParamArray': [['[', ']'], ['ε']],
        'ParamTypesList': [[',', 'Type', 'ID', 'OptionalParamArray', 'ParamTypesList'], ['ε']],
        'StatementList': [['Statement', 'StatementList'], ['ε']],
        'Statement': [
            ['if', '(', 'Expression', ')', 'Statement', 'OptionalElse'],
            ['while', '(', 'Expression', ')', 'Statement'],
            ['for', '(', 'OptionalAssign', ';', 'OptionalExpression', ';', 'OptionalAssign', ')', 'Statement'],
            ['return', 'OptionalExpression', ';'],
            ['ID', 'OptionalExprIndex', '=', 'Expression', ';'],
            ['ID', '(', 'OptionalExpressionList', ')', ';'],
            ['{', 'StatementList', '}'],
            [';']
        ],
        'OptionalElse': [['else', 'Statement']],
        'OptionalAssign': [['Assign'], ['ε']],
        'OptionalExpression': [['Expression'], ['ε']],
        'OptionalExpressionList': [['Expression', 'MoreExpressions'], ['ε']],
        'MoreExpressions': [[',', 'Expression', 'MoreExpressions'], ['ε']],
        'Assign': [['ID', 'OptionalExprIndex', '=', 'Expression']],
        'OptionalExprIndex': [['[', 'Expression', ']'], ['ε']],
        'Expression': [['LogAndExpr', 'ExpressionPrime']],
        'ExpressionPrime': [['||', 'LogAndExpr', 'ExpressionPrime'], ['ε']],
        'LogAndExpr': [['RelExpr', 'LogAndExprPrime']],
        'LogAndExprPrime': [['&&', 'RelExpr', 'LogAndExprPrime'], ['ε']],
        'RelExpr': [['AddExpr', 'RelExprPrime']],
        'RelExprPrime': [
            ['==', 'AddExpr', 'RelExprPrime'], ['!=', 'AddExpr', 'RelExprPrime'],
            ['<', 'AddExpr', 'RelExprPrime'], ['>', 'AddExpr', 'RelExprPrime'],
            ['<=', 'AddExpr', 'RelExprPrime'], ['>=', 'AddExpr', 'RelExprPrime'], ['ε']
        ],
        'AddExpr': [['MultExpr', 'AddExprPrime']],
        'AddExprPrime': [['+', 'MultExpr', 'AddExprPrime'], ['-', 'MultExpr', 'AddExprPrime'], ['ε']],
        'MultExpr': [['UnaryExpr', 'MultExprPrime']],
        'MultExprPrime': [['*', 'UnaryExpr', 'MultExprPrime'], ['/', 'UnaryExpr', 'MultExprPrime'], ['ε']],
        'UnaryExpr': [['-', 'UnaryExpr'], ['!', 'UnaryExpr'], ['Factor']],
        'Factor': [
            ['ID', 'ExpressionTail'], ['(', 'Expression', ')'],
            ['integerconstant'], ['charconstant'], ['stringconstant']
        ],
        'ExpressionTail': [['(', 'OptionalExpressionList', ')'], ['[', 'Expression', ']'], ['ε']],
    }

    print(f"\n{COR_OP}=== Gramática Original ==={RESET}")
    imprimir_gramatica(gramatica_linguagem)

    print(f"\n{COR_OP}=== 1. Tratamento da Gramática ==={RESET}")
    g_sem_recursao = eliminar_recursao_esquerda(gramatica_linguagem)
    g_final = fatorar_esquerda(g_sem_recursao)
    imprimir_gramatica(g_final)

    # Cálculos teóricos
    firsts = calcular_first(g_final)
    follows = calcular_follow(g_final, firsts, simbolo_inicial='Program')
    first_plus = calcular_first_plus(g_final, firsts, follows)

    print(f"\n{COR_OP}=== 2. Conjuntos FIRST+ por Produção ==={RESET}")
    imprimir_first_plus(first_plus, g_final)

    # Validação e Veredito final
    eh_ll1, lista_conflitos = verificar_se_eh_ll1(first_plus)

    print(f"\n{COR_OP}=== 3. Veredito do Parser ==={RESET}")
    if eh_ll1:
        print(f"{COR_SUCESSO}✅ SUCESSO! A gramática é estritamente LL(1).{RESET}")
        print("Você pode construir a tabela de análise preditiva sem nenhum conflito.")
    else:
        print(f"{COR_ERRO}❌ FALHA! A gramática NÃO é LL(1).{RESET}")
        print("Motivo: Existem conflitos de escolha para o mesmo Não-Terminal:\n")
        for conf in lista_conflitos:
            nt = conf['nao_terminal']
            p1 = ' '.join(conf['prod1'])
            p2 = ' '.join(conf['prod2'])
            print(f"No Não-Terminal [{COR_NT}{nt}{RESET}]:")
            print(f"  A regra ({COR_NT}{nt}{RESET} -> {p1}) e a regra ({COR_NT}{nt}{RESET} -> {p2})")
            print(f"  disputam o(s) mesmo(s) token(s) de entrada: {COR_ERRO}{conf['simbolos_conflito']}{RESET}\n")