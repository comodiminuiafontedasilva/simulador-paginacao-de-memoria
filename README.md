# Projeto 2: Simulador de Paginação de Memória

## 👤 Integrantes
- Bruna Soncini 10428267
- Julia Andrade 10427828
- Gabriel Buck 10425384
- Lucas Akio 10425346
- Henrique Catanha 10426708

## ✏️ Descrição do Projeto
O objetivo deste projeto é desenvolver um simulador que demonstre o funcionamento do mecanismo de paginação em sistemas operacionais.

## 🔧 Funcionalidades
1. Realizar a tradução de endereços virtuais para endereços físicos
2. Gerenciar tabelas de páginas para múltiplos processos
3. Implementar pelo menos dois algoritmos de substituição de páginas (FIFO E RANDOM)
4. Registrar e exibir estatísticas de desempenho (page faults, taxa de acertos, etc.)
5. Exibir o estado da memória física ao longo da simulação

## 👩‍💻 Como rodar o projeto
1. No terminal, usar o seguinte comando:
```gcc -o simulador src/*.c```

2. Isso vai compilar o programa e gerar o arquivo "simulador".
3. Para rodar esse arquivo, usar o seguinte comando no terminal:
```./simulador```

