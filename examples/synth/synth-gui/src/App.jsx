import {useState} from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import SliderComponent from "./SliderComponent.jsx";

function App() {
    const labelStyle = {
        "margin": "5vh",
        "padding": "0",
        "display": "flex",
        "color": "black",
        "fontSize": "10vw",
        "webkitTransform": "scale(0.65, 1)"
    }

    const sliderContainerStyle = {
        "margin": "0",
        "padding": "0",
        "display": "grid",
        "flexDirection": "row",
        "alignContent": "center",
        "justifyContent": "center",
        "alignItems": "center"
    }
    const sliderLabelStyle = {
        "top": "0",
        "margin": "0",
        "padding": "0",
        "color": "black",
        "fontSize": "5vw",
        "webkitTransform": "scale(0.65, 1)"
    }


    const attackParam = window.params["Attack"];
    const releaseParam = window.params["Release"];

    return (
        <>
            <div>
                <p style={labelStyle}>synth</p>
                <div style={sliderContainerStyle}>
                    <p style={sliderLabelStyle}>attack</p>
                    <SliderComponent pid={attackParam.pid}
                                     min={attackParam.min}
                                     max={attackParam.max}
                                     initial={attackParam.initial}
                    />
                </div>
                <div style={sliderContainerStyle}>
                    <p style={sliderLabelStyle}>release</p>
                    <SliderComponent pid={releaseParam.pid}
                                     min={releaseParam.min}
                                     max={releaseParam.max}
                                     initial={releaseParam.initial}
                    />
                </div>
            </div>
        </>
    )
}

export default App
