import {useState} from 'react'
import './App.css'
import SliderComponent from "./SliderComponent.jsx";

function App() {
    return (
        <>
            <div className="container">
                <p className="title">Delay</p>
                <div className="sliders">
                    <SliderComponent pid={window.params.kDelayTime} min={0.01} max={2.0} name={"Delay"}/>
                </div>
            </div>
        </>
    )
}

export default App
