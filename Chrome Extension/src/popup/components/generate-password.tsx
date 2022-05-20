import * as React from "react";
import FormGroup from "@mui/material/FormGroup";
import FormControlLabel from "@mui/material/FormControlLabel";
import Checkbox from "@mui/material/Checkbox";
import {
  Box,
  Button,
  FormControl,
  Grid,
  IconButton,
  Input,
  InputAdornment,
  InputLabel,
  OutlinedInput,
  Slider,
  styled,
  Tooltip,
  Typography,
} from "@mui/material";
import { useSnackbar, VariantType, SnackbarProvider } from "notistack";
import Visibility from "@mui/icons-material/Visibility";
import VisibilityOff from "@mui/icons-material/VisibilityOff";
import { useState } from "react";
import { generatePassword } from "../../utils/api";
import ContentCopy from "@mui/icons-material/ContentCopy";
import Numbers from "@mui/icons-material/Numbers";

export default function GeneratePasswordElement() {
  return <CheckboxLabels />;
}

function CheckboxLabels() {
  const [showPassword, setShowPassword] = useState(false);
  const [upper, setUpper] = useState(true);
  const [special, setSpecial] = useState(true);
  const [numbers, setNumbers] = useState(true);
  const [password, setPassword] = useState("");
  const [value, setValue] = React.useState<
    number | string | Array<number | string>
  >(30);

  const handleUpperChange = (event) => {
    setUpper(event.target.checked);
  };

  const handleSpecialChange = (event) => {
    setSpecial(event.target.checked);
  };

  const handleNumbersChange = (event) => {
    setNumbers(event.target.checked);
  };

  const handleClickShowPassword = () => {
    setShowPassword(!showPassword);
  };

  const handleMouseDownPassword = (
    event: React.MouseEvent<HTMLButtonElement>
  ) => {
    event.preventDefault();
  };

  // const Input = styled(MuiInput)`
  //   width: 42px;
  // `;

  function InputSlider() {
    const handleSliderChange = (event: Event, newValue: number | number[]) => {
      setValue(newValue);
    };

    const handleInputChange = (event: React.ChangeEvent<HTMLInputElement>) => {
      setValue(event.target.value === "" ? "" : Number(event.target.value));
    };

    const handleBlur = () => {
      if (value < 0) {
        setValue(0);
      } else if (value > 500) {
        setValue(500);
      }
    };

    return (
      <Box sx={{ width: "95%" }}>
        <Typography id="input-slider" gutterBottom>
          Number of Characters
        </Typography>
        <Grid container spacing={2} alignItems="center">
          <Grid item>
            <Numbers />
          </Grid>
          <Grid item xs>
            <Slider
              value={typeof value === "number" ? value : 0}
              onChange={handleSliderChange}
              aria-labelledby="input-slider"
              min={0}
              max={500}
            />
          </Grid>
          <Grid item>
            <Input
              value={value}
              size="small"
              onChange={handleInputChange}
              onBlur={handleBlur}
              inputProps={{
                step: 10,
                min: 10,
                max: 500,
                type: "number",
                "aria-labelledby": "input-slider",
              }}
            />
          </Grid>
        </Grid>
      </Box>
    );
  }

  function MyApp() {
    const handleClick = () => {
      generatePassword(upper, special, numbers, Number(value))
        .then((res) => {
          setPassword(res.generated);
        })
        .catch((err) => {
          console.log(err);
        });
    };

    return (
      <React.Fragment>
        <Button variant="contained" onClick={handleClick}>
          Generate
        </Button>
      </React.Fragment>
    );
  }

  function IntegrationNotistack() {
    return (
      <SnackbarProvider maxSnack={3}>
        <MyApp />
      </SnackbarProvider>
    );
  }

  return (
    <Box
      sx={{ display: "flex", flexWrap: "wrap", maxWidth: 400, maxHeight: 400 }}
    >
      <FormControl sx={{ m: 1, width: "55ch" }} variant="outlined">
        <FormControlLabel
          control={<Checkbox defaultChecked />}
          label="A-Z"
          onChange={() => handleUpperChange(event)}
        />
        <FormControlLabel
          control={<Checkbox defaultChecked />}
          label="0-9"
          onChange={() => handleNumbersChange(event)}
        />
        <FormControlLabel
          control={<Checkbox defaultChecked />}
          label="-_.:;,?&%$!@#"
          onChange={() => handleSpecialChange(event)}
        />
      </FormControl>

      <FormControl sx={{ m: 1, width: "55ch" }} variant="outlined">
        <InputSlider />
      </FormControl>

      <FormControl sx={{ m: 1, width: "55ch" }} variant="outlined">
        <IntegrationNotistack />
      </FormControl>

      <FormControl sx={{ m: 1, width: "55ch" }} variant="outlined">
        <InputLabel htmlFor="outlined-adornment-password">Password</InputLabel>
        <OutlinedInput
          id="outlined-adornment-password"
          type={showPassword ? "text" : "password"}
          readOnly={true}
          value={password}
          endAdornment={
            <InputAdornment position="end">
              <IconButton
                aria-label="toggle password visibility"
                onClick={handleClickShowPassword}
                onMouseDown={handleMouseDownPassword}
                edge="end"
              >
                {showPassword ? <VisibilityOff /> : <Visibility />}
              </IconButton>
              <Tooltip title="Copy">
                <IconButton
                  size="small"
                  onClick={() => {
                    navigator.clipboard.writeText(password);
                  }}
                >
                  <ContentCopy />
                </IconButton>
              </Tooltip>
            </InputAdornment>
          }
          label="Password"
        />
      </FormControl>
    </Box>
  );
}
