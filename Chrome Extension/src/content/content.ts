import { getAllPasswordsByHostname, ApiBody } from "../utils/api";
import { getStoredOptions, LocalStorageOptions } from "../utils/storage";

function nextField() {
  var inputs = document.getElementsByTagName("input");
  for (var i = 0; i < inputs.length; i++) {
    if (document.activeElement.id == inputs[i].id && i + 1 < inputs.length) {
      return inputs[i + 1];
    }
  }
  return undefined;
}

/** @type {Element | null} */
let lastFocusedElement = null;


// Set an interval without any period so it is polled at the higher resolution possible
setInterval(() => {
  getStoredOptions().then((options) => {
    if (!options.is_locked && options.is_autocomplete_enabled) {
      let focusedElement = document.activeElement;
      if (focusedElement !== lastFocusedElement) {
        if (focusedElement.tagName.toLowerCase() == "input") {
          if (
            (focusedElement as HTMLInputElement).type.toLowerCase() !=
            "password"
          ) {
            let nextInput = nextField();
            if (nextInput.type.toLowerCase() == "password") {
              /* Username + Password fields */
              console.log(
                "Found username and password fields for: " +
                  window.location.hostname.replace("www.", "")
              );

              // Get the username and password from the API
              getAllPasswordsByHostname(
                window.location.hostname.replace("www.", "")
              ).then((data: ApiBody) => {
                console.log(data);
                // get the stored options
                // if auxtocomplete is enabled and there is only one stored password for the current hostname
                if (data.count == 1) {
                  // Set the username and password
                  if (focusedElement instanceof HTMLInputElement) {
                    focusedElement.value = data.passwords[0].username;
                  }
                  nextInput.value = data.passwords[0].password;
                  (document.activeElement as HTMLInputElement).blur();
                }
              });
            }
          } else {
            /* Only password field */
            // Get the element with the type password
            console.log(
              "Found only password field for: " +
                window.location.hostname.replace("www.", "")
            );

            // Get the username and password from the API
            getAllPasswordsByHostname(
              window.location.hostname.replace("www.", "")
            ).then((data: ApiBody) => {
              console.log(data);
              // get the stored options
              // if autocomplete is enabled and there is only one stored password for the current hostname
              if (data.count == 1) {
                if (focusedElement instanceof HTMLInputElement) {
                  focusedElement.value = data.passwords[0].password;
                  (document.activeElement as HTMLInputElement).blur();
                }
              }
            });
          }
          lastFocusedElement = focusedElement;
        }
      }
    }
  });
});
