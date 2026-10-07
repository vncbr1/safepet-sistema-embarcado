# SafePet

**Protótipo acadêmico de coleira para pets com cerca virtual simulada e leitura de aceleração.**

Equipe: Marcos Vinícius, Jailton Alves, Guilherme Monteiro e Gabriel Cordeiro.
Empresa acadêmica proposta: **SafePet Tecnologia** (nome a confirmar com a equipe).

## Problema e proposta
Tutores podem demorar a perceber que um animal saiu de uma área definida. A proposta é demonstrar a lógica de uma cerca virtual e um alerta local. A hipótese deve ser discutida com o professor antes de fechar o problema, como exige o enunciado. Não há pesquisa com clientes documentada.

## O que funciona e o que é simulado
- ESP32 processa entradas e controla OLED e buzzer.
- Revisão adiciona MPU6050 para leitura de aceleração. Esse sensor NÃO mede localização.
- Latitude e longitude entram por comandos no monitor serial, substituindo um receptor GNSS somente na demonstração.
- Cerca circular: alerta acima de 100 m, retorno até 80 m. Faixa intermediária preserva o estado (histerese).
- Sem entrada inicial ou após 30 segundos sem nova posição: SEM POSICAO. Não há declaração de segurança nessa condição.
- Sem GPS real, aplicativo, mapa, internet, notificação remota ou autonomia de bateria demonstrada.

## Conteúdo
- `original/`: cópia preservada dos três arquivos do Wokwi da equipe.
- `firmware/`: sketch revisado, lógica da cerca, circuito com sensor e bibliotecas.
- `tests/`: testes C++ da lógica compartilhada com o firmware.
- `docs/`: revisão, empresa/equipe, roteiro de demonstração, testes e instruções de Git.

Projeto original: https://wokwi.com/projects/476617832747866113
A versão revisada está em `firmware/`. O endereço acima é a referência do esboço inicial; o link da cópia revisada deve ser acrescentado pela equipe.

## Executar no Wokwi
1. Abra o projeto original e salve uma cópia na conta da equipe.
2. Substitua `sketch.ino`, `diagram.json` e `libraries.txt` pelo conteúdo de `firmware/`.
3. Crie a aba `geofence.h` se ela não existir e cole o arquivo de mesmo nome. Se a aba já existir, substitua o conteúdo. O upload do Wokwi rejeita arquivos com nomes já existentes; nesse caso, copie e cole o conteúdo em cada aba.
4. Inicie a simulação. O estado inicial esperado é `SEM POSICAO`.
5. No monitor serial, envie `c` e Enter, depois `f` e Enter, e novamente `c` e Enter.
6. Clique no MPU6050 e altere a aceleração X para 1 g. O valor `Delta a` deve subir. Esse teste não simula GPS.

Se faltarem bibliotecas, use o Library Manager e adicione as cinco de `libraries.txt`. A compilação integrada da revisão e os testes de OLED/buzzer/sensor no Wokwi precisam ser concluídos pela equipe antes de apresentar.

## Comandos
| Comando | Efeito |
|---|---|
| `h` | Mostra ajuda |
| `c` | Posição simulada da base |
| `f` | Posição simulada aproximadamente 222,4 m ao norte |
| `p -8.0872 -34.8775` | Entrada manual (~89 m da base) |
| `s` | Invalida a posição |

Use letras minúsculas, sem traço antes do comando, e ponto decimal. Envie Enter ao final de cada comando. Uma linha vazia não altera o estado. Entradas inválidas não renovam a posição. O comando `c` representa uma leitura da base, não redefine o centro da cerca.

## Testes automatizados em computador
Com compilador C++ instalado, na raiz do repositório:
```bash
g++ -std=c++11 -Wall -Wextra -pedantic tests/test_geofence.cpp -o /tmp/test_safepet
/tmp/test_safepet
```
O executável usa o mesmo `geofence.h` do firmware. Os testes passaram em 07/10/2026 e verificam distância, limites, histerese e entradas inválidas. Eles não substituem compilação ESP32 nem validação de hardware.

## Estado da revisão
O original iniciou no Wokwi e o envio de `x` disparou fuga. A análise estática identificou ausência de sensor, de cálculo de distância e de retorno ao estado seguro. A revisão resolve esses pontos no código e no diagrama entregues. Na primeira revisão, a importação automatizada falhou. Posteriormente, Marcos informou que conseguiu carregar a revisão e compartilhou uma captura da ajuda no terminal. Isso evidencia execução do programa, mas não comprova aprovação de todos os testes de integração. A equipe deve registrar os resultados de cada caso em `docs/TESTES_E_DEMONSTRACAO.md`.

## Fontes técnicas
- https://docs.wokwi.com/parts/wokwi-mpu6050
- https://docs.wokwi.com/parts/wokwi-buzzer
- https://docs.wokwi.com/parts/board-ssd1306
- https://github.com/adafruit/Adafruit_MPU6050

## Próximos passos
Confirmar problema e nome com o professor, concluir e registrar os testes de integração. Para produto real, integrar GNSS e comunicação com o tutor, tratar perda de sinal, medir consumo e projetar um invólucro confortável. Buzzer junto ao animal exige avaliação de conforto e não deve servir para punição.

## Documentação para apresentação
- [Relatório técnico em PDF](docs/SafePet_Relatorio.pdf)
- [Slides editáveis](docs/SafePet_Apresentacao.pptx)
- [Empresa e equipe](docs/EMPRESA_EQUIPE.md)
- [Testes e demonstração](docs/TESTES_E_DEMONSTRACAO.md)

O PDF e os slides registram a revisão inicial de 07/10/2026. As atualizações posteriores de execução estão neste README e no registro de testes.
