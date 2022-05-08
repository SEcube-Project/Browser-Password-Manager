import React from "react";
import ReactDOM from "react-dom";
import {
  Card,
  CardContent,
  Typography,
  CardActions,
  Button,
  Box,
  Grid,
  Switch,
  FormControlLabel,
  FormGroup,
  Stack,
} from "@mui/material";

export function BasicButtons() {
  return (
    <Stack spacing={2} direction="row">
      <Button variant="contained">Save</Button>
    </Stack>
  );
}

export function OptionsPageHeader() {
  return (
    <Card>
      <CardContent>
        <Typography variant="h5" component="h2">
          Options
        </Typography>
        <Typography variant="body2" component="p">
          This is the options page
        </Typography>
      </CardContent>
    </Card>
  );
}

export function SwitchLabels(text: string) {
  return (
    <FormGroup>
      <FormControlLabel control={<Switch defaultChecked />} label={text} />
    </FormGroup>
  );
}
