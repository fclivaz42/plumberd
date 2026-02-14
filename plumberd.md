lowkey need this if i want to pull it off and not forget

client side: can either subscribe/listen or execute/command
both read/send buffers are capped at 512 bytes tops

`l v` becomes `listen volume`
`l b` becomes `listen brightness`
server responds with the currently held value for initialization
then sends each corresponding value every time it is updated
sends error string + closes() connection in case of issue

`x v <args>` becomes `execute volume <args>`
`x b <args>` becomes `execute brightness <args>`
`x k <args>` becomes `execute keyboard <args>`
server responds `ok` in case of success or error string
(if bytes read > 2 we have a problem) then immediately closes the socket
client closes the socket as soon as something is received
