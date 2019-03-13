# wake-lock

😴🙅

Prevent your laptop from sleeping. (macOS only for now.)

## Usage

```js
const {WakeLock} = require('wake-lock')

const lock = new WakeLock("Reticulating splines")

// ... reticulate the splines, which process must not be interrupted ...

lock.release()
```

## API

### `new WakeLock(reasonForLocking, {type: "idle" | "display"})`

Creates a new wake lock which will prevent the machine from going to sleep due
to idle detection until the lock is released. Forced sleep, e.g. from closing
the laptop's lid or a low battery emergency, will not be prevented. Multiple
wake locks are allowed: the machine will stay awake until all wake locks are
released.

* `reasonForLocking` String - a descriptive string used by the system whenever
  it needs to tell the user why the system is not sleeping. For example, "Mail
  Compacting Mailboxes" would be a useful string.
* `options` Object _(optional)_
  * `type` "idle" | "display" - if "display" is specified, display sleep will
    be prevented. If "idle" is specified (the default), the display will sleep
    but the machine will stay awake while the lock is held.

### `WakeLock.release()`

Releases a wake lock, allowing the machine to sleep. Calling this function on a
lock that has already been released will throw an error.
