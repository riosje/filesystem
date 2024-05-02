const { watch } = require('fs')

const path = "./";
const abortController = new AbortController();
const events = [];
const watcher = watch(
    path,
    {signal: abortController.signal},
    (event, path) => events.push({event, path})
);
watcher.addListener("error", error => void events.push({event: "Error", error}));
// If you insert a delay here, then sometimes the test will pass on macOS.
// The longer the delay, the more likely the test is to pass.
// You can insert a delay here if you want, but it doesn't seem to make any difference.
setInterval(() => console.log(events), 1000).unref()
setTimeout(() => {
  console.log(events);
  abortController.abort()
}, 60000);