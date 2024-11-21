import {useState} from 'react'
import './App.css'
import SliderComponent from "./SliderComponent.jsx";

function App() {
    const timeParam = window.params["Time"];
    const feedbackParam = window.params["Feedback"];
    const dryWetParam = window.params["Dry/Wet"];

    return (
        <>
            <div>
                <p className="label">delay</p>
                <SliderComponent pid={timeParam.id}
                                 min={timeParam.min}
                                 max={timeParam.max}
                                 initial={timeParam.initial}
                />
                <SliderComponent pid={feedbackParam.id}
                                 min={feedbackParam.min}
                                 max={feedbackParam.max}
                                 initial={feedbackParam.initial}
                />
                <SliderComponent pid={dryWetParam.id}
                                 min={dryWetParam.min}
                                 max={dryWetParam.max}
                                 initial={dryWetParam.initial}
                />
            </div>
        </>
    )
}

export default App
