# plumberd
### _Because overengineering tiling wm's feels better than spamming half-baked bash scripts to tie everything together._

`plumberd` is a simple utility: it does the plumbing between [Hyprland](https://github.com/hyprwm/Hyprland) and [Elkowar's Wacky Widgets](https://github.com/elkowar/eww).
Essentially, it "transforms" any keybinds or actions that would open/close popups (volume, brightness, notifications, you call it) that are not listen-able to in a listen-able way.
Instead of polling for battery percentage (even those eww cannot see), `plumberd` sends a notification to each listening client connected to it containing the value of the battery.

### Okay yeah, but why?

I could not be asked to learn QML and set quickshell back up when EWW already had what I wanted (plus, my themes are in GTK and not Qt).
I also could not be asked to set up too many hacky scripts to provide functionality that is considered basic in other DE's or even other OS'es.
I also wanted something fast, performant and easy to setup/use; therefore i came up with `plumberd` which can be extended with more functionality as time goes by.

### How does it work?

`plumberd` is written in C++ because the class system allows to quickly spin up modules that can be set up and used in the server. `plumberd` is both the client and the server, depending on how you start it.
Server-side: running `plumberd s` will launch the server daemon in the background. It creates a socket file the client side will connect itself to.
Running `plumberd l < v | b >` will connect to the server and listen to any changes made to volume or brightness. Any changes will be sent from the server to any client listening and each client will just receive the current volume or brightness, in %.
Each "module" should handle both the client functions (subscribing) as well as the server functions (listening on other services, formatting the reply); the server only receives the request and forwards it to each connected client.
A wireless, bluetooth and battery update notifiers are being planned.

### All of that just for brightness and volume...?

As stupid as this sounds, this isn't really *just about* setting brightness and volume levels. One of the goals is to make the whole system expandable with more and more functionality; storage, RAM, CPU usage, wlan, bluetooth, who knows.
This approach also allows to make bouncier animations (transitions) for sliders and properly handle pop-up opening and closing without having to bother with calling `eww update`, `eww get` or who knows a million times in a row or handling weird behavior for eg. timeouts (since holding vol-up will spam multiple instances of the same program, i wanted to offload all of that onto a server to not have to figure out which spawned process should be the one closing the popup OR have tens of processes just sitting around for a few seconds doing basically nothing).

The other thing is that its not *only* for the pop-ups; anything sent by `plumberd` can be reused in... any other component or even program that could use those values! It's not entirely tied to Hyprland and Eww, it's more of a glue that can tie multiple things together, hence its name. It does the plumbing to make twm's work with other components.

Essentially, `plumberd` started as a `volumed` shell script in my dotfiles and eventually evolved into what it is. It's just supposed to notify instead of letting widget apps run periodic polls on things that can be listen-able. **And, most importantly, allows me to dive deeper into the Linux desktop software stack.** Maybe someone already made a tool similar to mine, but I am using this to learn more about the inner workings of Linux!
