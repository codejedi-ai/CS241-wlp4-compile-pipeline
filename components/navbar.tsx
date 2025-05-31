"use client"

import Link from "next/link"
import { usePathname } from "next/navigation"
import { Button } from "@/components/ui/button"
import { Code, Users, Calendar, Trophy } from "lucide-react"
import Image from "next/image"

export default function Navbar() {
  const pathname = usePathname()

  return (
    <header className="fixed top-0 left-0 right-0 z-50 bg-[#13111c]/95 backdrop-blur-sm border-b border-purple-900/20">
      <div className="container mx-auto py-4 px-4 flex justify-between items-center">
        <div className="flex items-center space-x-3">
          <Link href="/" className="flex items-center space-x-3">
            <Image src="/logo.png" alt="Code Civilization Logo" width={40} height={40} className="rounded-lg" />
            <span className="text-2xl font-bold text-purple-500">Code Civilization</span>
          </Link>
        </div>
        <nav className="hidden md:flex items-center space-x-8">
          <Link
            href="/"
            className={`${pathname === "/" ? "text-white" : "text-white/80"} hover:text-white transition-colors flex items-center space-x-1`}
          >
            <span>Home</span>
          </Link>
          <Link href="/levels" className="text-white/80 hover:text-white transition-colors flex items-center space-x-1">
            <Code className="h-4 w-4" />
            <span>Levels</span>
          </Link>
          <Link
            href="/contests"
            className="text-white/80 hover:text-white transition-colors flex items-center space-x-1"
          >
            <Trophy className="h-4 w-4" />
            <span>Contests</span>
          </Link>
          <Link
            href="/community"
            className="text-white/80 hover:text-white transition-colors flex items-center space-x-1"
          >
            <Users className="h-4 w-4" />
            <span>Community</span>
          </Link>
          <Link href="/daily" className="text-white/80 hover:text-white transition-colors flex items-center space-x-1">
            <Calendar className="h-4 w-4" />
            <span>Daily Challenge</span>
          </Link>
        </nav>
        <div className="flex items-center space-x-4">
          <Link href="/signin" className="text-white/90 hover:text-white transition-colors">
            Sign In
          </Link>
          <Link href="/get-started">
            <Button className="bg-purple-600 hover:bg-purple-700 text-white">Join Civilization</Button>
          </Link>
        </div>
      </div>
    </header>
  )
}
