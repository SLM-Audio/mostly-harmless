import {useState} from 'react'
import './App.css'
import SliderComponent from "./SliderComponent.jsx";

function App() {
    return (
        <>
            <div>
                <p className="label">gain</p>
                <SliderComponent pid={window.params.kGain.id}
                                 min={window.params.kGain.min}
                                 max={window.params.kGain.max}
                                 initial={window.params.kGain.initial}
                />
            </div>
        </>
    )
}

export default App
