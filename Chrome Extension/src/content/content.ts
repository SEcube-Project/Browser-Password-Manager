import { getAllPasswordsByHostname, ApiBody } from "../utils/api";
import { getStoredOptions, LocalStorageOptions } from "../utils/storage";

// (() => {
//   function nextField() {
//     var inputs = document.getElementsByTagName("input");
//     for (var i = 0; i < inputs.length; i++) {
//       if (document.activeElement.id == inputs[i].id && i + 1 < inputs.length) {
//         return inputs[i + 1];
//       }
//     }
//     return undefined;
//   }

//   /** @type {Element | null} */
//   let lastFocusedElement = null;

//   setInterval(() => {
//     let focusedElement = document.activeElement;
//     if (focusedElement !== lastFocusedElement) {
//       if (focusedElement.tagName.toLowerCase() == "input") {
//         if ((focusedElement as HTMLInputElement).type.toLowerCase() != "password") {
//           let nextInput = nextField();
//           if (nextInput.type.toLowerCase() == "password") {
//             /* Username + Password fields */
//             console.log(
//               "Found username and password fields for: " +
//                 window.location.hostname.replace("www.", "")
//             );

//             // Get the username and password from the API
//             getAllPasswordsByHostname(window.location.hostname.replace("www.", "")).then(
//               (data: ApiBody) => {
//                 console.log(data);
//                 // get the stored options
//                 getStoredOptions().then((options: LocalStorageOptions) => {
//                   // if auxtocomplete is enabled and there is only one stored password for the current hostname
//                   if (
//                     data.count == 1 &&
//                     options.is_autocomplete_enabled
//                   ) {
//                     // Set the username and password
//                     if (focusedElement instanceof HTMLInputElement) {
//                       focusedElement.value = data.passwords[0].username;
//                     }
//                     nextInput.value = data.passwords[0].password;
//                     (document.activeElement as HTMLInputElement).blur()
//                   }
//                 });
//               }
//             );
//           }
//         } else {
//             /* Only password field */
//             // Get the element with the type password
//             console.log(
//               "Found only password field for: " + window.location.hostname.replace("www.", "")
//             );

//             // Get the username and password from the API
//             getAllPasswordsByHostname(window.location.hostname.replace("www.", "")).then(
//               (data: ApiBody) => {
//                 console.log(data);
//                 // get the stored options
//                 getStoredOptions().then((options: LocalStorageOptions) => {
//                   // if autocomplete is enabled and there is only one stored password for the current hostname
//                   if (
//                     data.count == 1 &&
//                     options.is_autocomplete_enabled
//                   ) {
//                     if (focusedElement instanceof HTMLInputElement) {
//                       focusedElement.value = data.passwords[0].password;
//                       (document.activeElement as HTMLInputElement).blur()
//                     }
//                   }
//                 });
//               }
//             );
//           }
//           lastFocusedElement = focusedElement;
//         }
//       }
//   });
// })();

// // Search for all input fields in the current page
// const inputs = document.getElementsByTagName("input");
// console.log(inputs);
// // Loop through all input fields
// for (let i = 0; i < inputs.length; i++) {
//   // If the input field is a password field
//   if (inputs[i].type.toLowerCase() == "password") {
//     // Add a listener to the input field
//     inputs[i].addEventListener("keyup", (event) => {
//       // If the input field is focused
//       if (inputs[i] == document.activeElement) {
//         // Get the stored options
//         getStoredOptions().then((options: LocalStorageOptions) => {
//           // If autocomplete is enabled
//           if (options.is_autocomplete_enabled) {
//             // Get the username and password from the API
//             getAllPasswordsByHostname(window.location.hostname.replace("www.", "")).then(
//               (data: ApiBody) => {
//                 // If there is only one stored password for the current hostname
//                 if (data.count == 1) {
//                   // Set the username and password
//                   inputs[i].value = data.passwords[0].username;
//                   inputs[i + 1].value = data.passwords[0].password;
//                 }
//               }
//             );
//           }
//         });
//       }
//     });
//   }
// }

getStoredOptions().then((options) => {
  if (options.is_autocomplete_enabled && !options.is_locked) {
    // make a fetch to https://www.7timer.info/bin/astro.php?lon=113.2&lat=23.1&ac=0&unit=metric&output=json&tzshift=0
    fetch("https://www.7timer.info/bin/astro.php?lon=113.2&lat=23.1&ac=0&unit=metric&output=json&tzshift=0")
      .then((response) => response.json())
      .then((data) => {
        console.log(data);
      }
      )
      .catch((error) => {
        console.log(error);
      }
      );
  } else {
    console.log("background2.ts");
  }
}
);

