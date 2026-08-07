# Eiwa — Roadmap: Lacunas da Linguagem e Stdlib

**Data:** 2026-08  
**Fonte:** aprendizado do `example/arest` (HTTP + MCP / JSON-RPC). Lista apenas o que ainda precisa ser feito. Bugs do compilador/backend diferenciados de features ausentes.

---

## Fase A — Compilador / backend (de maior impacto)

| # | Item | Tipo | Detalhe |
|---|---|---|---|
| 1 | Named args com defaults pulados em construtores de struct | bug (C backend) | `JsonValue(kind = ..., obj = ...)` transpilou com args posicionais trocados (int no slot de `Map*`). Libera constructores ergonômicos (remove factories). |
| 2 | Smart-cast em cadeias `&&` com nullable | bug/limitação | `if (v != null && v.isString())` não estreita `v`; exige `val node = v!!`. Contradiz o tour (seções 5/10). |
| 3 | Diagnóstico de erros C cru | QoL | Erros de transpilação aparecem como `RAW C ERROR LINE` mapeado para `main.ei` mesmo quando a origem é outro módulo; dificulta localizar o ponto real. |

## Fase B — Stdlib fundamental

| # | Item | Detalhe |
|---|---|---|
| 4 | `String.toInt()` | Existe `toDouble()` mas não `toInt()`; inteiros exigem `text.toDouble().toInt()`. |
| 5 | `indexOf(needle, fromIndex)` e `lastIndexOf` | Só há `indexOf(needle)` de 1 arg; truques com `substring` para "buscar de X". |
| 6 | `StringBuilder` / concat eficiente | `+` de string recria buffers (malloc a cada junção); falta acumulador para JSON/HTTP pesado. |
| 7 | `break` / `continue` em loops | Não existem; loops usam flags booleanas (`while (scanning) { ... scanning = false }`). |
| 8 | `for (i in 0..n)` range | Não há ranges numéricos; só `for (item in list)` — força `while` manual. |

## Fase C — JSON completo em `std.json`

| # | Item | Detalhe |
|---|---|---|
| 9 | `fromJson` tipado (desserialização) | Desbloqueia `argument<T>(name)`. Requer unir com o sistema `Serde*`/`Serializable` / type-class. |
| 10 | Edge cases do parser | Números negativos, exponencial, `\uXXXX` reais (hoje `?`), números sem fração, strings com `\b`. |

---

## Anexo — Gambiarras a reverter quando a linguagem melhorar

- Factories `jsonObject(...)` → construtor com named args (depende da Fase A item 1).
- Flags `scanning`/`trimming` em loops → `break` (depende da Fase B item 7).