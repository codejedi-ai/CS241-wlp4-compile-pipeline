"use client"

import { signIn } from "next-auth/react"
import { Button } from "@/asdasd/components/ui/button"

export default function SignIn() {
  return (
    <div className="flex flex-col items-center justify-center min-h-screen bg-gray-100">
      <div className="p-8 bg-white rounded-lg shadow-md">
        <h1 className="text-2xl font-bold mb-4">Sign in to Discord Auth App</h1>
        <Button 
          onClick={() => signIn("discord", { callbackUrl: "/" })}
          className="bg-[#7289da] hover:bg-[#5f73bc]"
        >
          Sign in with Discord
        </Button>
      </div>
    </div>
  )
}

