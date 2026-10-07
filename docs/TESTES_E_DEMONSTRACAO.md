# Testes e demonstração

## Evidência efetivamente obtida em 07/10/2026
- Original: inicialização observada no terminal Wokwi.
- Original: `x` seguido de Enter produziu "Evento de fuga simulado!".
- Revisão: testes C++ do arquivo `geofence.h` passaram para distância, limites 80/100 m, histerese, parser e preservação de coordenadas em erro.
- Revisão inicial: importação automatizada falhou. Depois, Marcos informou que carregou os arquivos e enviou captura da ajuda no terminal, evidenciando execução. Os casos abaixo continuam sem registro individual de aprovação. Não há medição física de alcance, precisão ou bateria.

## Testes de integração a executar
| ID | Procedimento | Resultado esperado | Estado |
|---|---|---|---|
| T01 | Iniciar com os quatro arquivos revisados | Compila e mostra SEM POSICAO | Pendente |
| T02 | Enviar c | DENTRO, 0 m, sem alarme | Pendente |
| T03 | Enviar f | FORA, ~222,4 m, bipe intermitente | Pendente |
| T04 | Depois de f, enviar c | Volta a DENTRO e silencia | Pendente |
| T05 | Enviar x, linha vazia, p 91 0 | Não gera fuga nem modifica posição | Pendente |
| T06 | c, depois p -8.0872 -34.8775 | ~89 m, conserva DENTRO | Pendente |
| T07 | f, depois p -8.0872 -34.8775 | ~89 m, conserva FORA | Pendente |
| T08 | Esperar 30 s de TEMPO SIMULADO sem entrada | SEM POSICAO e sem bipe | Pendente |
| T09 | MPU: X=1g, Y=0g, Z=1g | Delta a ~4,1 m/s² | Pendente |
| T10 | Remover MPU do diagrama e reiniciar | MPU: FALHA, cerca ainda responde | Pendente |
| T11 | Enviar linha de 100 caracteres e Enter, depois c | Rejeita excesso e aceita c | Pendente |
| T12 | Enviar s após f | SEM POSICAO e alarme cessa | Pendente |

Registrar data, observação e captura de tela ao executar. No Wokwi lento, 30 s simulados podem demorar muito mais que 30 s reais. No teste T08, perda de posição significa estado desconhecido, nunca retorno comprovado à área.

## Roteiro de demonstração (sugestão de 2 minutos)
1. Explicar ESP32, display, buzzer e sensor. Declarar que GPS é simulado.
2. Iniciar e mostrar SEM POSICAO.
3. Enviar c, f e c, esperando o display responder em cada etapa.
4. Alterar a aceleração do MPU6050 e mostrar Delta a.
5. Enviar x para mostrar rejeição e s para demonstrar posição desconhecida.
6. Mostrar repositório com código e documentação.

Preparar uma gravação curta de uma execução bem-sucedida como contingência. Não apresentar a gravação como execução ao vivo.
