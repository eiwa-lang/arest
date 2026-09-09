# Arest — Framework Roadmap & Arquitetura

**Versão Atual:** 0.1.0  
**Linguagem Base:** Eiwa (>= 0.0.56)

---

## 🎯 Visão Geral
O **Arest** é o framework web e MCP (Model Context Protocol) nativo para a linguagem Eiwa. Inspirado no ecossistema Ktor / FastAPI, ele oferece:
1. **HTTP Client:** cliente assíncrono flexível com engines intercambiáveis (`CurlEngine`, `MockEngine`) e negociação de conteúdo tipada (`ContentNegotiation`).
2. **HTTP Server:** servidor HTTP não-bloqueante orientado a corrotinas stackless com roteamento declarativo e arquivos estáticos.
3. **MCP Server:** servidor completo do protocolo MCP (JSON-RPC 2.0) para exposição de ferramentas (*tools*), recursos (*resources*) e *prompts* para agentes de IA.

---

## 🚀 Status das Fases

### Fase 0: Fundações & Cliente HTTP (CONCLUÍDA)
- [x] **ArestClient Core:** Fluent DSL para construção de requisições (`get`, `post`, `put`, `patch`, `delete`, `head`).
- [x] **Engines:** `CurlEngine` para requisições nativas de rede e `MockEngine` para testes unitários sem rede.
- [x] **Content Negotiation & Serde:** Deserialização protocol-agnostic tipada com `res.body<T>()` usando `std.serde` e `Json`.
- [x] **Headers Reais:** Captura e parsing robusto de headers de resposta no `CurlEngine`.
- [x] **String Templates:** Migração integral de concatenações de string para interpolação nativa `$var` e `${expr}`.
- [x] **Suíte de Testes do Cliente:** 11/11 testes passando (`client_mock_test.ei`, `client_curl_test.ei`).

---

### Fase 1: Servidor HTTP Multi-Core com `Dispatchers` (CONCLUÍDA)
Aproveita a arquitetura de corrotinas com paralelismo real (Phase 69 do Eiwa).
- [x] **Task 1.1 — Dispatcher Padrão Multi-Core:** Configurar o accept loop do servidor em `arest.ei` para usar `task(disp) { dispatcher.dispatch(conn) }`.
- [x] **Task 1.2 — Configuração no Builder:** Permitir configurar o dispatcher no `ArestBuilder` (`dispatcher(Dispatchers.Default)` por padrão, com opção para `Dispatchers.Single` ou pools dedicados).
- [x] **Task 1.3 — Testes de Servidor Concorrente:** Teste automatizado em `tests/server_multicore_test.ei` validando `Dispatchers.Default`, pool customizado e conexões concorrentes.

---

### Fase 2: Ergonomia do Servidor — Serde & Helpers no `ApplicationCall` (CONCLUÍDA)
- [x] **Task 2.1 — `call.respond(value: Serializable)`:** Permitir passar diretamente instâncias com `: Serializable` sem exigir `.toJson()` manual ou acoplamento a JSON, usando `ContentNegotiation`.
- [x] **Task 2.2 — `call.receive<T>()`:** Desserialização direta do corpo da requisição para tipos tipados usando o sistema `deserialize<T>` do `std.serde` via negociação.
- [x] **Task 2.3 — Request Helpers:** Métodos `call.header(name): String?` (com fallback case-insensitive), `call.query(name): String?` e `call.queryOrDefault(name, default): String`.
- [x] **Task 2.4 — Testes Automatizados:** Suíte `tests/server_serde_test.ei` validando round-trip completo de `respond(Serializable)` e `receive<T>()`.

---

### Fase 3: Roteamento com Parâmetros de Path (PENDENTE)
- [x] **Task 3.1 — Path Matching Dinâmico:** Suporte a parâmetros de rota na DSL (`get("/users/:id")` ou `get("/users/{id}")`).
- [x] **Task 3.2 — `call.pathParam(name): String?`:** Extração determinística de parâmetros de caminho mapeados pelo roteador.
- [x] **Task 3.3 — Tratamento de Rotas Conflitantes:** Ordem de prioridade (rotas estáticas exatas antes de rotas com curinga/parâmetro).

---

### Fase 4: Ergonomia de Ferramentas MCP (`McpCall`) (PENDENTE)
- [ ] **Task 4.1 — Argumentos Tipados:**
  - `call.argumentString(name): String?`
  - `call.argumentInt(name): Int?`
  - `call.argumentDouble(name): Double?`
  - `call.argumentBool(name): Bool?`
- [ ] **Task 4.2 — Validação Automática de Schema:** Validar required/types dos argumentos antes de invocar o handler da ferramenta.

---

### Fase 5: Middlewares & Interceptors (PENDENTE)
- [ ] **Task 5.1 — Pipeline de Interceptors:** Suporte a interceptores pré e pós-despacho no `ArestBuilder`.
- [ ] **Task 5.2 — CORS Middleware:** `builder.cors { ... }` para cabeçalhos `Access-Control-Allow-*` e resposta automática a `OPTIONS`.
- [ ] **Task 5.3 — StatusPages & Tratamento de Erros:** `builder.statusPages { ... }` para capturar exceções ou respostas 404/500 customizadas em JSON.

---

### Fase 6: Suíte de Testes do Servidor e MCP (PENDENTE)
- [ ] **Task 6.1 — Testes de Roteamento:** Handlers HTTP (GET, POST, PUT, DELETE), 404, health check e arquivos estáticos.
- [ ] **Task 6.2 — Testes do Protocolo MCP:** Ciclo JSON-RPC (`initialize`, `tools/list`, `tools/call`, `resources`, `prompts`).