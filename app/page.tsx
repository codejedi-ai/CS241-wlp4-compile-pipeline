import { Button } from "@/components/ui/button"
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card"
import { Badge } from "@/components/ui/badge"
import { Trophy, Code, Users, Zap, Shield, Target, ArrowRight, CheckCircle, XCircle } from "lucide-react"
import Link from "next/link"
import Image from "next/image"

export default function Home() {
  return (
    <main>
      {/* Hero Section */}
      <section className="container mx-auto py-24 px-4">
        <div className="text-center space-y-8">
          <div className="space-y-4">
            <Badge variant="outline" className="border-purple-500/50 text-purple-400">
              Daily Programming Challenges
            </Badge>
            <div className="flex items-center justify-center space-x-4">
              <Image src="/logo.png" alt="Code Civilization Logo" width={80} height={80} className="rounded-lg" />
              <h1 className="text-5xl md:text-7xl font-bold text-white">
                Code <span className="text-purple-500">Civilization</span>
              </h1>
            </div>
            <p className="text-xl text-white/80 max-w-3xl mx-auto">
              Join a community of developers who solve coding problems daily. Progress through tiers, unlock exclusive
              perks, and build your programming skills one challenge at a time.
            </p>
          </div>
          <div className="flex flex-wrap justify-center gap-4 pt-4">
            <Link href="/get-started">
              <Button size="lg" className="bg-purple-600 hover:bg-purple-700 text-white">
                Start Your Journey
                <ArrowRight className="ml-2 h-4 w-4" />
              </Button>
            </Link>
            <Link href="/levels">
              <Button
                size="lg"
                variant="outline"
                className="border-purple-500/50 text-purple-400 hover:bg-purple-500/10"
              >
                Explore Levels
              </Button>
            </Link>
          </div>
        </div>
      </section>

      {/* Tier System */}
      <section className="py-24 bg-gradient-to-b from-transparent to-purple-950/20">
        <div className="container mx-auto px-4">
          <div className="text-center mb-16">
            <h2 className="text-3xl font-bold text-purple-500 mb-4">Choose Your Path</h2>
            <p className="text-white/70 max-w-2xl mx-auto">
              Progress through three tiers of difficulty. Each tier offers unique challenges and exclusive benefits.
            </p>
          </div>

          <div className="grid grid-cols-1 md:grid-cols-3 gap-8">
            {/* Easy Tier */}
            <Card className="bg-[#1a1025] border-green-500/30 relative overflow-hidden">
              <div className="absolute top-0 left-0 right-0 h-1 bg-green-500"></div>
              <CardHeader className="text-center">
                <div className="mx-auto mb-4 p-3 bg-green-500/10 rounded-full w-fit">
                  <Code className="h-8 w-8 text-green-500" />
                </div>
                <CardTitle className="text-2xl text-green-500">Easy Tier</CardTitle>
                <CardDescription className="text-white/60">Perfect for beginners</CardDescription>
              </CardHeader>
              <CardContent className="space-y-4">
                <div className="space-y-2">
                  <h4 className="font-semibold text-white">Daily Requirement:</h4>
                  <p className="text-white/70">1 Easy problem per day</p>
                </div>
                <div className="space-y-2">
                  <h4 className="font-semibold text-white">Perks:</h4>
                  <ul className="text-white/70 space-y-1">
                    <li className="flex items-center">
                      <CheckCircle className="h-4 w-4 text-green-500 mr-2" />
                      Platform membership access
                    </li>
                    <li className="flex items-center">
                      <CheckCircle className="h-4 w-4 text-green-500 mr-2" />
                      Basic problem library
                    </li>
                    <li className="flex items-center">
                      <CheckCircle className="h-4 w-4 text-green-500 mr-2" />
                      Community discussions
                    </li>
                  </ul>
                </div>
                <div className="pt-4">
                  <Button className="w-full bg-green-600 hover:bg-green-700">Start Easy</Button>
                </div>
              </CardContent>
            </Card>

            {/* Medium Tier */}
            <Card className="bg-[#1a1025] border-yellow-500/30 relative overflow-hidden">
              <div className="absolute top-0 left-0 right-0 h-1 bg-yellow-500"></div>
              <CardHeader className="text-center">
                <div className="mx-auto mb-4 p-3 bg-yellow-500/10 rounded-full w-fit">
                  <Target className="h-8 w-8 text-yellow-500" />
                </div>
                <CardTitle className="text-2xl text-yellow-500">Medium Tier</CardTitle>
                <CardDescription className="text-white/60">For intermediate developers</CardDescription>
              </CardHeader>
              <CardContent className="space-y-4">
                <div className="space-y-2">
                  <h4 className="font-semibold text-white">Daily Requirement:</h4>
                  <p className="text-white/70">1 Medium problem per day</p>
                </div>
                <div className="space-y-2">
                  <h4 className="font-semibold text-white">Perks:</h4>
                  <ul className="text-white/70 space-y-1">
                    <li className="flex items-center">
                      <CheckCircle className="h-4 w-4 text-yellow-500 mr-2" />
                      Job board access
                    </li>
                    <li className="flex items-center">
                      <CheckCircle className="h-4 w-4 text-yellow-500 mr-2" />
                      Coffee chats with professionals
                    </li>
                    <li className="flex items-center">
                      <CheckCircle className="h-4 w-4 text-yellow-500 mr-2" />
                      Resume reviews
                    </li>
                  </ul>
                </div>
                <div className="pt-4">
                  <Button className="w-full bg-yellow-600 hover:bg-yellow-700">Join Medium</Button>
                </div>
              </CardContent>
            </Card>

            {/* Hard Tier */}
            <Card className="bg-[#1a1025] border-red-500/30 relative overflow-hidden">
              <div className="absolute top-0 left-0 right-0 h-1 bg-red-500"></div>
              <CardHeader className="text-center">
                <div className="mx-auto mb-4 p-3 bg-red-500/10 rounded-full w-fit">
                  <Trophy className="h-8 w-8 text-red-500" />
                </div>
                <CardTitle className="text-2xl text-red-500">Hard Tier</CardTitle>
                <CardDescription className="text-white/60">Elite level challenges</CardDescription>
              </CardHeader>
              <CardContent className="space-y-4">
                <div className="space-y-2">
                  <h4 className="font-semibold text-white">Daily Requirement:</h4>
                  <p className="text-white/70">1 Hard problem per day</p>
                </div>
                <div className="space-y-2">
                  <h4 className="font-semibold text-white">Perks:</h4>
                  <ul className="text-white/70 space-y-1">
                    <li className="flex items-center">
                      <CheckCircle className="h-4 w-4 text-red-500 mr-2" />
                      Exclusive job opportunities
                    </li>
                    <li className="flex items-center">
                      <CheckCircle className="h-4 w-4 text-red-500 mr-2" />
                      Direct company connections
                    </li>
                    <li className="flex items-center">
                      <CheckCircle className="h-4 w-4 text-red-500 mr-2" />
                      Platform referrals
                    </li>
                  </ul>
                </div>
                <div className="pt-4">
                  <Button className="w-full bg-red-600 hover:bg-red-700">Reach Hard</Button>
                </div>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>

      {/* Level Progression */}
      <section className="py-24">
        <div className="container mx-auto px-4">
          <div className="text-center mb-16">
            <h2 className="text-3xl font-bold text-purple-500 mb-4">Level Progression System</h2>
            <p className="text-white/70 max-w-2xl mx-auto">
              Advance through tiers by competing in weekly contests and maintaining daily streaks.
            </p>
          </div>

          <div className="grid grid-cols-1 lg:grid-cols-2 gap-12">
            <Card className="bg-[#1a1025] border-purple-500/30">
              <CardHeader>
                <CardTitle className="text-purple-500 flex items-center">
                  <ArrowRight className="h-5 w-5 mr-2" />
                  Advancement Requirements
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-6">
                <div className="space-y-3">
                  <h4 className="font-semibold text-green-500">Easy → Medium</h4>
                  <p className="text-white/70">Complete weekly contest: 2 Easy + 2 Medium problems</p>
                  <p className="text-sm text-white/50">Must solve ALL 4 problems to advance</p>
                </div>
                <div className="space-y-3">
                  <h4 className="font-semibold text-yellow-500">Medium → Hard</h4>
                  <p className="text-white/70">Complete weekly contest: 2 Medium + 2 Hard problems</p>
                  <p className="text-sm text-white/50">Must solve ALL 4 problems to advance</p>
                </div>
                <div className="space-y-3">
                  <h4 className="font-semibold text-purple-500">Contest Requirements</h4>
                  <p className="text-white/70">Finish in top 10 of weekly programming contests</p>
                  <p className="text-sm text-white/50">Dynamic difficulty ensures fair distribution</p>
                </div>
              </CardContent>
            </Card>

            <Card className="bg-[#1a1025] border-red-500/30">
              <CardHeader>
                <CardTitle className="text-red-500 flex items-center">
                  <XCircle className="h-5 w-5 mr-2" />
                  Strike System & Demotion
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-6">
                <div className="space-y-3">
                  <h4 className="font-semibold text-white">Daily Check-ins</h4>
                  <p className="text-white/70">Missing daily problems results in strikes</p>
                  <p className="text-sm text-white/50">Make up missed days to clear strikes</p>
                </div>
                <div className="space-y-3">
                  <h4 className="font-semibold text-white">Inactivity Demotion</h4>
                  <p className="text-white/70">Extended inactivity moves you to inactive status</p>
                  <p className="text-sm text-white/50">Take placement test to determine new tier</p>
                </div>
                <div className="space-y-3">
                  <h4 className="font-semibold text-white">AI Usage Policy</h4>
                  <p className="text-white/70">Gen AI allowed for algorithms, not problem solutions</p>
                  <p className="text-sm text-white/50">Community verification prevents AI cheating</p>
                </div>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>

      {/* Features */}
      <section className="py-24 bg-gradient-to-b from-purple-950/20 to-transparent">
        <div className="container mx-auto px-4">
          <div className="text-center mb-16">
            <h2 className="text-3xl font-bold text-purple-500 mb-4">Platform Features</h2>
            <p className="text-white/70 max-w-2xl mx-auto">
              Everything you need to excel in competitive programming and land your dream job.
            </p>
          </div>

          <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-8">
            <Card className="bg-[#1a1025] border-purple-500/20">
              <CardHeader>
                <Trophy className="h-8 w-8 text-purple-500 mb-2" />
                <CardTitle>Weekly Contests</CardTitle>
              </CardHeader>
              <CardContent>
                <p className="text-white/70">
                  Compete in dynamic programming contests with targeted difficulty distribution.
                </p>
              </CardContent>
            </Card>

            <Card className="bg-[#1a1025] border-purple-500/20">
              <CardHeader>
                <Users className="h-8 w-8 text-purple-500 mb-2" />
                <CardTitle>Community Verification</CardTitle>
              </CardHeader>
              <CardContent>
                <p className="text-white/70">
                  Peer review system ensures authentic problem-solving and prevents AI abuse.
                </p>
              </CardContent>
            </Card>

            <Card className="bg-[#1a1025] border-purple-500/20">
              <CardHeader>
                <Zap className="h-8 w-8 text-purple-500 mb-2" />
                <CardTitle>Daily Challenges</CardTitle>
              </CardHeader>
              <CardContent>
                <p className="text-white/70">Curated problems from LeetCode, HackerRank, and Codeforces platforms.</p>
              </CardContent>
            </Card>

            <Card className="bg-[#1a1025] border-purple-500/20">
              <CardHeader>
                <Shield className="h-8 w-8 text-purple-500 mb-2" />
                <CardTitle>Job Opportunities</CardTitle>
              </CardHeader>
              <CardContent>
                <p className="text-white/70">
                  Access exclusive job boards and direct company referrals based on your tier.
                </p>
              </CardContent>
            </Card>

            <Card className="bg-[#1a1025] border-purple-500/20">
              <CardHeader>
                <Code className="h-8 w-8 text-purple-500 mb-2" />
                <CardTitle>Skill Tracking</CardTitle>
              </CardHeader>
              <CardContent>
                <p className="text-white/70">Monitor your progress with detailed analytics and streak tracking.</p>
              </CardContent>
            </Card>

            <Card className="bg-[#1a1025] border-purple-500/20">
              <CardHeader>
                <Target className="h-8 w-8 text-purple-500 mb-2" />
                <CardTitle>Mentorship</CardTitle>
              </CardHeader>
              <CardContent>
                <p className="text-white/70">
                  Connect with industry professionals through coffee chats and resume reviews.
                </p>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>

      {/* CTA Section */}
      <section className="bg-gradient-to-r from-purple-900 to-purple-600 py-16">
        <div className="container mx-auto px-4 text-center">
          <h2 className="text-3xl font-bold mb-4">Ready to Build Your Coding Civilization?</h2>
          <p className="text-white/80 max-w-2xl mx-auto mb-8">
            Join thousands of developers who are advancing their careers through daily practice and community support.
          </p>
          <Link href="/get-started">
            <Button size="lg" className="bg-white text-purple-900 hover:bg-white/90">
              Start Your Journey Today
            </Button>
          </Link>
        </div>
      </section>
    </main>
  )
}
