# Publicar no GitHub

Git controla versões. GitHub hospeda o repositório. Não compartilhe senha, token, código de autenticação ou chave privada em mensagens.

## Caminho simples pelo site
1. Entre no GitHub na sua própria conta.
2. Crie um repositório chamado `safepet-sistema-embarcado`.
3. Escolha público se a equipe concordar e o professor precisar de acesso aberto. Caso privado, combine o acesso com ele.
4. Extraia o ZIP entregue e envie o CONTEÚDO da pasta `safepet`, preservando `firmware`, `original`, `tests` e `docs`.
5. O `README.md` deve ficar na raiz. Não envie somente o ZIP: o professor precisa poder ler os arquivos.
6. Use uma mensagem de commit como `Documenta SafePet e adiciona firmware revisado`.
7. Confira os arquivos no repositório e abra o link fora da sessão, caso público.
8. Acrescente o link da cópia revisada do Wokwi ao README após salvá-la.

## Alternativa com terminal
Execute dentro da pasta extraída. Substitua SEU_USUARIO pelo usuário real e crie previamente um repositório vazio, sem README remoto.
```bash
git init
git add .
git commit -m "Documenta SafePet e adiciona firmware revisado"
git branch -M main
git remote add origin https://github.com/SEU_USUARIO/safepet-sistema-embarcado.git
git push -u origin main
```
Configure nome e e-mail do Git se solicitado. Faça autenticação segura no navegador ou gerenciador de credenciais. Não cole segredos no código ou no histórico.

## Colaboração
Adicionar colegas como colaboradores requer conhecer seus usuários do GitHub. Cada integrante pode trabalhar em uma branch e abrir um pull request. Registre contribuições reais. Não atribua trabalho retroativamente a quem não o fez.

## Publicação nesta entrega
Repositório público da equipe: https://github.com/vncbr1/safepet-sistema-embarcado

As instruções acima explicam a criação inicial. Para atualizações neste repositório, use commits sobre a cópia existente, sem executar novamente a criação do remoto.
